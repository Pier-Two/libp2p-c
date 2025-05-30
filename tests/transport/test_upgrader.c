#include "multiformats/multiaddr/multiaddr.h"
#include "peer_id/peer_id.h"
#include "peer_id/peer_id_ed25519.h"
#include "protocol/noise/protocol_noise.h"
#include "protocol/mplex/protocol_mplex.h"
#include "protocol/tcp/protocol_tcp.h"
#include "protocol/tcp/protocol_tcp_conn.h"
#include "transport/listener.h"
#include "transport/transport.h"
#include "transport/upgrader.h"
#include <fcntl.h>
#include <noise/protocol.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
#include <io.h>
#ifndef F_GETFL
#define F_GETFL 0
#endif
#ifndef F_SETFL
#define F_SETFL 0
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0
#endif
static inline int fcntl(int fd, int cmd, long arg)
{
    (void)fd;
    (void)cmd;
    (void)arg;
    return 0;
}
#endif

/* Simple test helper output */
static void print_standard(const char *name, const char *details, int passed)
{
    if (passed)
        printf("TEST: %-70s | PASS\n", name);
    else
        printf("TEST: %-70s | FAIL: %s\n", name, details);
}

static int failures = 0;
#define TEST_OK(name, cond, fmt, ...)                                                                                                                \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (cond)                                                                                                                                    \
            print_standard(name, "", 1);                                                                                                             \
        else                                                                                                                                         \
        {                                                                                                                                            \
            char _d[256];                                                                                                                            \
            snprintf(_d, sizeof(_d), fmt, ##__VA_ARGS__);                                                                                            \
            print_standard(name, _d, 0);                                                                                                             \
            failures++;                                                                                                                              \
        }                                                                                                                                            \
    } while (0)

void ed25519_genpub(uint8_t pub[32], const uint8_t sec[32]);

static int accept_with_timeout(libp2p_listener_t *lst, libp2p_conn_t **out, int attempts, int sleep_us)
{
    int rc = LIBP2P_LISTENER_ERR_AGAIN;
    for (int i = 0; i < attempts; i++)
    {
        rc = libp2p_listener_accept(lst, out);
        if (rc == 0)
            return 0;
        if (rc != LIBP2P_LISTENER_ERR_AGAIN)
            return rc;
        usleep(sleep_us);
    }
    return rc;
}

struct upg_args
{
    libp2p_upgrader_t *upg;
    libp2p_conn_t *conn;
    const peer_id_t *hint;
    libp2p_uconn_t *out;
    libp2p_upgrader_err_t rc;
};

static void *outbound_thread(void *arg)
{
    struct upg_args *a = arg;
    a->rc = libp2p_upgrader_upgrade_outbound(a->upg, a->conn, a->hint, &a->out);
    return NULL;
}

static void *inbound_thread(void *arg)
{
    struct upg_args *a = arg;
    a->rc = libp2p_upgrader_upgrade_inbound(a->upg, a->conn, &a->out);
    return NULL;
}

static int peer_id_from_ed25519_priv(const uint8_t *sk, peer_id_t *pid)
{
    uint8_t pub[32];
    ed25519_genpub(pub, sk);
    uint8_t *pubpb = NULL;
    size_t pubpb_len = 0;
    if (peer_id_build_public_key_protobuf(PEER_ID_ED25519_KEY_TYPE, pub, sizeof(pub), &pubpb, &pubpb_len) != PEER_ID_SUCCESS)
        return -1;
    int ret = peer_id_create_from_public_key(pubpb, pubpb_len, pid);
    free(pubpb);
    return ret == PEER_ID_SUCCESS ? 0 : -1;
}

static void test_upgrade_handshake(void)
{
    uint8_t static_cli[32];
    uint8_t static_srv[32];
    noise_randstate_generate_simple(static_cli, sizeof(static_cli));
    noise_randstate_generate_simple(static_srv, sizeof(static_srv));

    uint8_t id_cli[32] = {0x9d, 0x61, 0xb1, 0x9d, 0xef, 0xfd, 0x5a, 0x60, 0xba, 0x84, 0x4a, 0xf4, 0x92, 0xec, 0x2c, 0xc4,
                          0x44, 0x49, 0xc5, 0x69, 0x7b, 0x32, 0x69, 0x19, 0x70, 0x3b, 0xac, 0x03, 0x1c, 0xae, 0x7f, 0x60};
    uint8_t id_srv[32] = {0x4c, 0xcd, 0x08, 0x9b, 0x28, 0xff, 0x96, 0xda, 0x9d, 0xb6, 0xc3, 0x46, 0xec, 0x11, 0x4e, 0x0f,
                          0x5b, 0x8a, 0x31, 0x9f, 0x35, 0xab, 0xa6, 0x24, 0xda, 0x8c, 0xf6, 0xed, 0x4f, 0xb8, 0xa6, 0xfb};

    peer_id_t pid_cli = {0}, pid_srv = {0};
    TEST_OK("derive cli peer id", peer_id_from_ed25519_priv(id_cli, &pid_cli) == 0, "pid fail");
    TEST_OK("derive srv peer id", peer_id_from_ed25519_priv(id_srv, &pid_srv) == 0, "pid fail");

    libp2p_noise_config_t ncli = {.static_private_key = static_cli,
                                  .static_private_key_len = sizeof(static_cli),
                                  .identity_private_key = id_cli,
                                  .identity_private_key_len = sizeof(id_cli),
                                  .identity_key_type = PEER_ID_ED25519_KEY_TYPE};
    libp2p_noise_config_t nsrv = {.static_private_key = static_srv,
                                  .static_private_key_len = sizeof(static_srv),
                                  .identity_private_key = id_srv,
                                  .identity_private_key_len = sizeof(id_srv),
                                  .identity_key_type = PEER_ID_ED25519_KEY_TYPE};
    libp2p_security_t *sec_cli = libp2p_noise_security_new(&ncli);
    libp2p_security_t *sec_srv = libp2p_noise_security_new(&nsrv);
    TEST_OK("noise security alloc", sec_cli && sec_srv, "sec_cli=%p sec_srv=%p", (void *)sec_cli, (void *)sec_srv);
    if (!sec_cli || !sec_srv)
        return;

    libp2p_security_t *sec_list_cli[] = {sec_cli, NULL};
    libp2p_security_t *sec_list_srv[] = {sec_srv, NULL};
    libp2p_muxer_t *mux = libp2p_mplex_new();
    libp2p_muxer_t *mux_list[] = {mux, NULL};
    libp2p_upgrader_config_t uc = libp2p_upgrader_config_default();
    uc.security = (const libp2p_security_t *const *)sec_list_cli;
    uc.n_security = 1;
    uc.muxers = (const libp2p_muxer_t *const *)mux_list;
    uc.n_muxers = 1;
    libp2p_upgrader_t *up_cli = libp2p_upgrader_new(&uc);
    uc.security = (const libp2p_security_t *const *)sec_list_srv;
    uc.n_security = 1;
    uc.muxers = (const libp2p_muxer_t *const *)mux_list;
    uc.n_muxers = 1;
    libp2p_upgrader_t *up_srv = libp2p_upgrader_new(&uc);
    TEST_OK("upgrader alloc", up_cli && up_srv, "up_cli=%p up_srv=%p", (void *)up_cli, (void *)up_srv);
    if (!up_cli || !up_srv)
        return;

    int port = 9000 + (rand() % 1000);
    char addr_str[64];
    snprintf(addr_str, sizeof(addr_str), "/ip4/127.0.0.1/tcp/%d", port);
    int ma_err = 0;
    multiaddr_t *addr = multiaddr_new_from_str(addr_str, &ma_err);
    TEST_OK("multiaddr parse", addr && ma_err == 0, "err=%d", ma_err);

    libp2p_transport_t *tcp = libp2p_tcp_transport_new(NULL);
    libp2p_listener_t *lst = NULL;
    int rc = libp2p_transport_listen(tcp, addr, &lst);
    TEST_OK("listener create", rc == 0 && lst, "rc=%d", rc);

    libp2p_conn_t *cli = NULL;
    rc = libp2p_transport_dial(tcp, addr, &cli);
    TEST_OK("dial", rc == 0 && cli, "rc=%d", rc);

    libp2p_conn_t *srv = NULL;
    rc = accept_with_timeout(lst, &srv, 100, 2000);
    TEST_OK("accept", rc == 0 && srv, "rc=%d", rc);

    tcp_conn_ctx_t *cctx = cli->ctx;
    int flags = fcntl(cctx->fd, F_GETFL, 0);
    fcntl(cctx->fd, F_SETFL, flags & ~O_NONBLOCK);
    tcp_conn_ctx_t *sctx = srv->ctx;
    flags = fcntl(sctx->fd, F_GETFL, 0);
    fcntl(sctx->fd, F_SETFL, flags & ~O_NONBLOCK);

    struct upg_args cli_args = {.upg = up_cli, .conn = cli, .hint = NULL, .out = NULL};
    struct upg_args srv_args = {.upg = up_srv, .conn = srv, .hint = NULL, .out = NULL};
    pthread_t t_cli, t_srv;
    pthread_create(&t_cli, NULL, outbound_thread, &cli_args);
    pthread_create(&t_srv, NULL, inbound_thread, &srv_args);
    pthread_join(t_cli, NULL);
    pthread_join(t_srv, NULL);

    TEST_OK("upgrade return codes", cli_args.rc == LIBP2P_UPGRADER_OK && srv_args.rc == LIBP2P_UPGRADER_OK, "cli=%d srv=%d", cli_args.rc,
            srv_args.rc);

    struct libp2p_upgraded_conn *cu = (struct libp2p_upgraded_conn *)cli_args.out;
    struct libp2p_upgraded_conn *su = (struct libp2p_upgraded_conn *)srv_args.out;
    (void)cu;
    (void)su; /* silence unused if not used */
    printf("debug: cli=%p srv=%p\n", (void *)cli, (void *)srv);
    TEST_OK("remote peer verified", cli_args.out && cli_args.out->remote_peer && peer_id_equals(cli_args.out->remote_peer, &pid_srv) == 1, "match=%d",
            cli_args.out && cli_args.out->remote_peer ? peer_id_equals(cli_args.out->remote_peer, &pid_srv) : -1);
    TEST_OK("server saw client", srv_args.out && srv_args.out->remote_peer && peer_id_equals(srv_args.out->remote_peer, &pid_cli) == 1, "match=%d",
            srv_args.out && srv_args.out->remote_peer ? peer_id_equals(srv_args.out->remote_peer, &pid_cli) : -1);

    libp2p_mplex_ctx_t *ctx_c = libp2p_mplex_ctx_new(cli_args.out->conn);
    libp2p_mplex_ctx_t *ctx_s = libp2p_mplex_ctx_new(srv_args.out->conn);
    TEST_OK("mplex ctx", ctx_c && ctx_s, "ctx");
    if (ctx_c && ctx_s)
    {
        uint64_t sid = 0;
        TEST_OK("stream open", libp2p_mplex_stream_open(ctx_c, (const uint8_t *)"s", 1, &sid) == LIBP2P_MPLEX_OK,
                "open");
        TEST_OK("stream send", libp2p_mplex_stream_send(ctx_c, sid, 1, (const uint8_t *)"ping", 4) == LIBP2P_MPLEX_OK,
                "send");
        libp2p_mplex_process_one(ctx_s);
        libp2p_mplex_process_one(ctx_s);
        uint8_t buf[8];
        size_t n = 0;
        libp2p_mplex_err_t mrc = libp2p_mplex_stream_recv(ctx_s, sid, 0, buf, sizeof(buf), &n);
        TEST_OK("data after upgrade", mrc == LIBP2P_MPLEX_OK && n == 4 && memcmp(buf, "ping", 4) == 0,
                "mrc=%d n=%zu", mrc, n);
        libp2p_mplex_ctx_free(ctx_c);
        libp2p_mplex_ctx_free(ctx_s);
    }

    /* raw connections are owned by the upgraded conns now */
    if (cli_args.out)
    {
        if (cli_args.out->conn)
        {
            libp2p_conn_close(cli_args.out->conn);
            libp2p_conn_free(cli_args.out->conn);
        }
        if (cli_args.out->remote_peer)
        {
            peer_id_destroy(cli_args.out->remote_peer);
            free(cli_args.out->remote_peer);
        }
        free(cli_args.out);
    }
    if (srv_args.out)
    {
        if (srv_args.out->conn)
        {
            libp2p_conn_close(srv_args.out->conn);
            libp2p_conn_free(srv_args.out->conn);
        }
        if (srv_args.out->remote_peer)
        {
            peer_id_destroy(srv_args.out->remote_peer);
            free(srv_args.out->remote_peer);
        }
        free(srv_args.out);
    }
    libp2p_listener_close(lst);
    libp2p_transport_close(tcp);
    libp2p_transport_free(tcp);
    multiaddr_free(addr);
    libp2p_upgrader_free(up_cli);
    libp2p_upgrader_free(up_srv);
    libp2p_muxer_free(mux);
    libp2p_security_free(sec_cli);
    libp2p_security_free(sec_srv);
    peer_id_destroy(&pid_cli);
    peer_id_destroy(&pid_srv);
}

int main(void)
{
    srand((unsigned)time(NULL));
    test_upgrade_handshake();
    if (failures)
        printf("\nSome tests failed - total failures: %d\n", failures);
    else
        printf("\nAll upgrader tests passed!\n");
    return failures ? 1 : 0;
}
