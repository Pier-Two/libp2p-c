#include <noise/protocol.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "multiformats/multiaddr/multiaddr.h"
#include "multiformats/multicodec/multicodec_codes.h"
#include "peer_id/peer_id.h"
#include "peer_id/peer_id_ed25519.h"
#include "peer_id/peer_id_secp256k1.h"
#include "protocol/mplex/protocol_mplex.h"
#include "protocol/multiselect/protocol_multiselect.h"
#include "protocol/noise/protocol_noise.h"
#include "protocol/ping/protocol_ping.h"
#include "protocol/protocol_handler.h"
#include "protocol/tcp/protocol_tcp.h"
#include "protocol/yamux/protocol_yamux.h"
#include "transport/connection.h"
#include "transport/transport.h"
#include "transport/upgrader.h"

// Constants for test configuration
#define WAIT_TIMEOUT_SECONDS 30
#define RESPONSE_TIMEOUT_SECONDS 5
#define WAIT_INTERVAL_MS 10
#define RESPONSE_WAIT_ITERATIONS_PER_SECOND 100

// Structure to hold ping test context
typedef struct
{
    int ping_completed;
    uint64_t rtt;
    libp2p_ping_err_t ping_result;
} test_ping_ctx_t;

static void logmsg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    fflush(stdout);
}

static const char* upgrader_err_to_string(libp2p_upgrader_err_t err)
{
    switch (err) {
        case LIBP2P_UPGRADER_OK:
            return "OK";
        case LIBP2P_UPGRADER_ERR_NULL_PTR:
            return "NULL_PTR - Required pointer was NULL";
        case LIBP2P_UPGRADER_ERR_TIMEOUT:
            return "TIMEOUT - Handshake deadline expired";
        case LIBP2P_UPGRADER_ERR_SECURITY:
            return "SECURITY - No mutually supported security protocol";
        case LIBP2P_UPGRADER_ERR_MUXER:
            return "MUXER - No mutually supported muxer protocol";
        case LIBP2P_UPGRADER_ERR_HANDSHAKE:
            return "HANDSHAKE - Security handshake failed";
        case LIBP2P_UPGRADER_ERR_INTERNAL:
            return "INTERNAL - Unexpected internal failure";
        default:
            return "UNKNOWN";
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <multiaddr>\n", argv[0]);
        return 1;
    }

    // Variable declarations
    libp2p_security_t *noise = NULL;
    libp2p_muxer_t *mplex = NULL;
    libp2p_muxer_t *yamux = NULL;
    libp2p_upgrader_t *upgrader = NULL;
    libp2p_protocol_handler_registry_t *registry = NULL;
    libp2p_protocol_handler_ctx_t *handler_ctx = NULL;
    libp2p_conn_t *raw_conn = NULL;
    libp2p_uconn_t *uconn = NULL;
    char *host = NULL;

    logmsg("=== Enhanced Ping Protocol Test (High-Level API) ===\n");
    logmsg("connecting to: %s\n", argv[1]);

    // Parse multiaddr
    int err = 0;
    multiaddr_t *maddr = multiaddr_new_from_str(argv[1], &err);
    if (!maddr || err != 0) {
        fprintf(stderr, "failed to parse multiaddr: %s\n", argv[1]);
        return 1;
    }

    // Extract connection info using proper multiaddr API
    int port = 0;

    // Check that we have at least 2 protocols (ip4 + tcp)
    size_t nprotocols = multiaddr_nprotocols(maddr);
    if (nprotocols < 2) {
        fprintf(stderr, "multiaddr must have at least ip4 and tcp protocols\n");
        multiaddr_free(maddr);
        return 1;
    }

    // Extract IP address and port using multiaddr API
    uint64_t proto_code;
    uint8_t addr_bytes[16]; // Max size for IPv6
    size_t addr_len;

    // Find IP4 protocol and extract address
    bool found_ip = false;
    for (size_t i = 0; i < nprotocols; i++) {
        if (multiaddr_get_protocol_code(maddr, i, &proto_code) == 0) {
            if (proto_code == MULTICODEC_IP4) {
                addr_len = sizeof(addr_bytes);
                if (multiaddr_get_address_bytes(maddr, i, addr_bytes, &addr_len) == 0 && addr_len == 4) {
                    // Convert IPv4 bytes to string
                    host = malloc(16); // Enough for "xxx.xxx.xxx.xxx\0"
                    if (host) {
                        snprintf(host, 16, "%u.%u.%u.%u", addr_bytes[0], addr_bytes[1], addr_bytes[2], addr_bytes[3]);
                        found_ip = true;
                    }
                }
                break;
            }
        }
    }

    // Find TCP protocol and extract port
    bool found_tcp = false;
    for (size_t i = 0; i < nprotocols; i++) {
        if (multiaddr_get_protocol_code(maddr, i, &proto_code) == 0) {
            if (proto_code == MULTICODEC_TCP) {
                addr_len = sizeof(addr_bytes);
                if (multiaddr_get_address_bytes(maddr, i, addr_bytes, &addr_len) == 0 && addr_len == 2) {
                    // Convert port bytes (big-endian) to integer
                    port = (addr_bytes[0] << 8) | addr_bytes[1];
                    found_tcp = true;
                }
                break;
            }
        }
    }

    if (!found_ip || !found_tcp || !host || port == 0) {
        fprintf(stderr, "failed to extract IP4/TCP address from multiaddr\n");
        goto cleanup;
    }

    logmsg("connecting to %s:%d\n", host, port);

    // Create TCP transport and dial
    libp2p_tcp_config_t tcfg = libp2p_tcp_config_default();
    libp2p_transport_t *tcp = libp2p_tcp_transport_new(&tcfg);
    if (!tcp) {
        fprintf(stderr, "failed to create TCP transport\n");
        goto cleanup;
    }

    if (libp2p_transport_dial(tcp, maddr, &raw_conn) != 0) {
        fprintf(stderr, "failed to dial TCP connection\n");
        libp2p_transport_free(tcp);
        goto cleanup;
    }

    libp2p_transport_free(tcp); // We can free the transport after dialing
    logmsg("raw TCP connection established\n");

    // Generate identity keys
    static uint8_t static_key[32];
    static uint8_t identity_key[32];

    // Generate random keys directly
    noise_randstate_generate_simple(static_key, sizeof(static_key));
    noise_randstate_generate_simple(identity_key, sizeof(identity_key));

    libp2p_noise_config_t ncfg = {
        .static_private_key = static_key,
        .static_private_key_len = sizeof(static_key),
        .identity_private_key = identity_key,
        .identity_private_key_len = sizeof(identity_key),
        .identity_key_type = PEER_ID_SECP256K1_KEY_TYPE,
        .max_plaintext = 0
    };

    noise = libp2p_noise_security_new(&ncfg);
    yamux = libp2p_yamux_new();
    mplex = libp2p_mplex_new();

    const libp2p_security_t *sec[] = { noise, NULL };
    const libp2p_muxer_t *mux[] = { yamux, mplex, NULL };

    // Create and configure upgrader
    libp2p_upgrader_config_t config = libp2p_upgrader_config_default();
    config.security = sec;
    config.n_security = 1;
    config.muxers = mux;
    config.n_muxers = 2;
    config.handshake_timeout_ms = WAIT_TIMEOUT_SECONDS * 1000;

    upgrader = libp2p_upgrader_new(&config);
    if (!upgrader) {
        fprintf(stderr, "failed to create upgrader\n");
        goto cleanup;
    }

    logmsg("starting connection upgrade...\n");
    logmsg("- security protocols available: %zu (noise)\n", config.n_security);
    logmsg("- muxer protocols available: %zu (yamux, mplex)\n", config.n_muxers);
    
    // Upgrade the connection (security + muxing)
    libp2p_upgrader_err_t upgrade_err = libp2p_upgrader_upgrade_outbound(upgrader, raw_conn, NULL, &uconn);
    if (upgrade_err != LIBP2P_UPGRADER_OK) {
        fprintf(stderr, "upgrade failed: %s (error code: %d)\n", 
                upgrader_err_to_string(upgrade_err), upgrade_err);
        
        if (upgrade_err == LIBP2P_UPGRADER_ERR_HANDSHAKE) {
            fprintf(stderr, "  -> Security handshake failed during noise negotiation\n");
            fprintf(stderr, "  -> This could indicate:\n");
            fprintf(stderr, "     * Remote peer doesn't support /noise protocol\n");
            fprintf(stderr, "     * Multiselect negotiation failed\n");
            fprintf(stderr, "     * Noise cryptographic handshake failed\n");
            fprintf(stderr, "     * Network issues during handshake\n");
            fprintf(stderr, "     * Incompatible noise implementation\n");
            fprintf(stderr, "  -> Try connecting to a different libp2p node or check if remote supports Noise\n");
        } else if (upgrade_err == LIBP2P_UPGRADER_ERR_MUXER) {
            fprintf(stderr, "  -> Muxer negotiation failed\n");
            fprintf(stderr, "  -> This could indicate:\n");
            fprintf(stderr, "     * Remote peer doesn't support yamux or mplex\n");
            fprintf(stderr, "     * Network issues during muxer negotiation\n");
        } else if (upgrade_err == LIBP2P_UPGRADER_ERR_SECURITY) {
            fprintf(stderr, "  -> No security protocols configured or supported\n");
        }
        
        goto cleanup;
    }

    logmsg("connection upgraded successfully\n");
    logmsg("- noise handshake completed\n");
    logmsg("- muxer negotiation completed\n");

    // Create protocol handler registry
    registry = libp2p_protocol_handler_registry_new();
    if (!registry) {
        fprintf(stderr, "failed to create protocol handler registry\n");
        goto cleanup;
    }

    // Create protocol handler context
    handler_ctx = libp2p_protocol_handler_ctx_new(registry, uconn);
    if (!handler_ctx) {
        fprintf(stderr, "failed to create protocol handler context\n");
        goto cleanup;
    }

    // Start protocol handler thread to listen for incoming streams
    if (libp2p_protocol_handler_start(handler_ctx) != 0) {
        fprintf(stderr, "failed to start protocol handler\n");
        goto cleanup;
    }
    logmsg("protocol handler started, listening for incoming streams\n");

    // Perform ping test using the upgraded connection
    logmsg("Starting ping test\n");
    
    test_ping_ctx_t ping_ctx = {0};
    
    // Use the ping protocol through the muxed connection
    uint64_t rtt = 0;
    libp2p_ping_err_t ping_result = libp2p_ping_roundtrip(uconn->conn, 1000, &rtt);
    
    if (ping_result != LIBP2P_PING_OK) {
        fprintf(stderr, "ping failed (%d)\n", ping_result);
    } else {
        logmsg("✅ ping RTT %llu ms\n", (unsigned long long)rtt);
        ping_ctx.ping_completed = 1;
        ping_ctx.rtt = rtt;
        ping_ctx.ping_result = ping_result;
    }

cleanup:
    // Cleanup in reverse order of creation
    if (handler_ctx) {
        libp2p_protocol_handler_stop(handler_ctx);
        libp2p_protocol_handler_ctx_free(handler_ctx);
    }
    if (registry) {
        libp2p_protocol_handler_registry_free(registry);
    }
    if (uconn) {
        // Don't manually free the upgraded connection - let the upgrader handle it
        libp2p_conn_close(((struct libp2p_upgraded_conn*)uconn)->conn);
        free(uconn);
    }
    if (upgrader) {
        libp2p_upgrader_free(upgrader);
    }
    if (yamux) {
        libp2p_muxer_free(yamux);
    }
    if (mplex) {
        libp2p_muxer_free(mplex);
    }
    if (noise) {
        libp2p_security_free(noise);
    }
    free(host);
    multiaddr_free(maddr);

    logmsg("=== Test completed ===\n");
    return ping_ctx.ping_completed && ping_ctx.ping_result == LIBP2P_PING_OK ? 0 : 1;
}
