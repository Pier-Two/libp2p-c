#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "peer_id/peer_id.h"
#include "peer_id/peer_id_secp256k1.h"
#include "peer_id/peer_id_ed25519.h"
#include "peer_id/peer_id_ecdsa.h"
#include "peer_id/peer_id_rsa.h"

/* Helper function to print test results. */
static void print_standard(const char *test_name, const char *details, int passed)
{
    if (passed)
    {
        printf("TEST: %-50s | PASS\n", test_name);
    }
    else
    {
        printf("TEST: %-50s | FAIL: %s\n", test_name, details);
    }
}

/* Helper function: convert a hex string to a byte array.
   The caller must free the returned array.
*/
static uint8_t *hex_to_bytes(const char *hex, size_t *out_len)
{
    size_t hex_len = strlen(hex);
    if (hex_len % 2 != 0)
    {
        return NULL;
    }
    size_t bytes_len = hex_len / 2;
    uint8_t *bytes = malloc(bytes_len);
    if (!bytes)
    {
        return NULL;
    }
    for (size_t i = 0; i < bytes_len; i++)
    {
        char byte_str[3] = { hex[i * 2], hex[i * 2 + 1], '\0' };
        bytes[i] = (uint8_t)strtol(byte_str, NULL, 16);
    }
    *out_len = bytes_len;
    return bytes;
}

/* Test vectors (hex-encoded) for secp256k1 keys from the spec */
#define SECP256K1_PUBLIC_HEX "08021221037777e994e452c21604f91de093ce415f5432f701dd8cd1a7a6fea0e630bfca99"
#define SECP256K1_PRIVATE_HEX "0802122053DADF1D5A164D6B4ACDB15E24AA4C5B1D3461BDBD42ABEDB0A4404D56CED8FB"

/* Test vectors (hex-encoded) for RSA keys from the spec */
#define RSA_PRIVATE_HEX "080012ae123082092a0201000282020100e1beab071d08200bde24eef00d049449b07770ff9910257b2d7d5dda242ce8f0e2f12e1af4b32d9efd2c090f66b0f29986dbb645dae9880089704a94e5066d594162ae6ee8892e6ec70701db0a6c445c04778eb3de1293aa1a23c3825b85c6620a2bc3f82f9b0c309bc0ab3aeb1873282bebd3da03c33e76c21e9beb172fd44c9e43be32e2c99827033cf8d0f0c606f4579326c930eb4e854395ad941256542c793902185153c474bed109d6ff5141ebf9cd256cf58893a37f83729f97e7cb435ec679d2e33901d27bb35aa0d7e20561da08885ef0abbf8e2fb48d6a5487047a9ecb1ad41fa7ed84f6e3e8ecd5d98b3982d2a901b4454991766da295ab78822add5612a2df83bcee814cf50973e80d7ef38111b1bd87da2ae92438a2c8cbcc70b31ee319939a3b9c761dbc13b5c086d6b64bf7ae7dacc14622375d92a8ff9af7eb962162bbddebf90acb32adb5e4e4029f1c96019949ecfbfeffd7ac1e3fbcc6b6168c34be3d5a2e5999fcbb39bba7adbca78eab09b9bc39f7fa4b93411f4cc175e70c0a083e96bfaefb04a9580b4753c1738a6a760ae1afd851a1a4bdad231cf56e9284d832483df215a46c1c21bdf0c6cfe951c18f1ee4078c79c13d63edb6e14feaeffabc90ad317e4875fe648101b0864097e998f0ca3025ef9638cd2b0caecd3770ab54a1d9c6ca959b0f5dcbc90caeefc4135baca6fd475224269bbe1b02030100010282020100a472ffa858efd8588ce59ee264b957452f3673acdf5631d7bfd5ba0ef59779c231b0bc838a8b14cae367b6d9ef572c03c7883b0a3c652f5c24c316b1ccfd979f13d0cd7da20c7d34d9ec32dfdc81ee7292167e706d705efde5b8f3edfcba41409e642f8897357df5d320d21c43b33600a7ae4e505db957c1afbc189d73f0b5d972d9aaaeeb232ca20eebd5de6fe7f29d01470354413cc9a0af1154b7af7c1029adcd67c74b4798afeb69e09f2cb387305e73a1b5f450202d54f0ef096fe1bde340219a1194d1ac9026e90b366cce0c59b239d10e4888f52ca1780824d39ae01a6b9f4dd6059191a7f12b2a3d8db3c2868cd4e5a5862b8b625a4197d52c6ac77710116ebd3ced81c4d91ad5fdfbed68312ebce7eea45c1833ca3acf7da2052820eacf5c6b07d086dabeb893391c71417fd8a4b1829ae2cf60d1749d0e25da19530d889461c21da3492a8dc6ccac7de83ac1c2185262c7473c8cc42f547cc9864b02a8073b6aa54a037d8c0de3914784e6205e83d97918b944f11b877b12084c0dd1d36592f8a4f8b8da5bb404c3d2c079b22b6ceabfbcb637c0dbe0201f0909d533f8bf308ada47aee641a012a494d31b54c974e58b87f140258258bb82f31692659db7aa07e17a5b2a0832c24e122d3a8babcc9ee74cbb07d3058bb85b15f6f6b2674aba9fd34367be9782d444335fbed31e3c4086c652597c27104938b47fa10282010100e9fdf843c1550070ca711cb8ff28411466198f0e212511c3186623890c0071bf6561219682fe7dbdfd81176eba7c4faba21614a20721e0fcd63768e6d925688ecc90992059ac89256e0524de90bf3d8a052ce6a9f6adafa712f3107a016e20c80255c9e37d8206d1bc327e06e66eb24288da866b55904fd8b59e6b2ab31bc5eab47e597093c63fab7872102d57b4c589c66077f534a61f5f65127459a33c91f6db61fc431b1ae90be92b4149a3255291baf94304e3efb77b1107b5a3bda911359c40a53c347ff9100baf8f36dc5cd991066b5bdc28b39ed644f404afe9213f4d31c9d4e40f3a5f5e3c39bebeb244e84137544e1a1839c1c8aaebf0c78a7fad590282010100f6fa1f1e6b803742d5490b7441152f500970f46feb0b73a6e4baba2aaf3c0e245ed852fc31d86a8e46eb48e90fac409989dfee45238f97e8f1f8e83a136488c1b04b8a7fb695f37b8616307ff8a8d63e8cfa0b4fb9b9167ffaebabf111aa5a4344afbabd002ae8961c38c02da76a9149abdde93eb389eb32595c29ba30d8283a7885218a5a9d33f7f01dbdf85f3aad016c071395491338ec318d39220e1c7bd69d3d6b520a13a30d745c102b827ad9984b0dd6aed73916ffa82a06c1c111e7047dcd2668f988a0570a71474992eecf416e068f029ec323d5d635fd24694fc9bf96973c255d26c772a95bf8b7f876547a5beabf86f06cd21b67994f944e7a5493028201010095b02fd30069e547426a8bea58e8a2816f33688dac6c6f6974415af8402244a22133baedf34ce499d7036f3f19b38eb00897c18949b0c5a25953c71aeeccfc8f6594173157cc854bd98f16dffe8f28ca13b77eb43a2730585c49fc3f608cd811bb54b03b84bddaa8ef910988567f783012266199667a546a18fd88271fbf63a45ae4fd4884706da8befb9117c0a4d73de5172f8640b1091ed8a4aea3ed4641463f5ff6a5e3401ad7d0c92811f87956d1fd5f9a1d15c7f3839a08698d9f35f9d966e5000f7cb2655d7b6c4adcd8a9d950ea5f61bb7c9a33c17508f9baa313eecfee4ae493249ebe05a5d7770bbd3551b2eeb752e3649e0636de08e3d672e66cb90282010100ad93e4c31072b063fc5ab5fe22afacece775c795d0efdf7c704cfc027bde0d626a7646fc905bb5a80117e3ca49059af14e0160089f9190065be9bfecf12c3b2145b211c8e89e42dd91c38e9aa23ca73697063564f6f6aa6590088a738722df056004d18d7bccac62b3bafef6172fc2a4b071ea37f31eff7a076bcab7dd144e51a9da8754219352aef2c73478971539fa41de4759285ea626fa3c72e7085be47d554d915bbb5149cb6ef835351f231043049cd941506a034bf2f8767f3e1e42ead92f91cb3d75549b57ef7d56ac39c2d80d67f6a2b4ca192974bfc5060e2dd171217971002193dba12e7e4133ab201f07500a90495a38610279b13a48d54f0c99028201003e3a1ac0c2b67d54ed5c4bbe04a7db99103659d33a4f9d35809e1f60c282e5988dddc964527f3b05e6cc890eab3dcb571d66debf3a5527704c87264b3954d7265f4e8d2c637dd89b491b9cf23f264801f804b90454d65af0c4c830d1aef76f597ef61b26ca857ecce9cb78d4f6c2218c00d2975d46c2b013fbf59b750c3b92d8d3ed9e6d1fd0ef1ec091a5c286a3fe2dead292f40f380065731e2079ebb9f2a7ef2c415ecbb488da98f3a12609ca1b6ec8c734032c8bd513292ff842c375d4acd1b02dfb206b24cd815f8e2f9d4af8e7dea0370b19c1b23cc531d78b40e06e1119ee2e08f6f31c6e2e8444c568d13c5d451a291ae0c9f1d4f27d23b3a00d60ad"
#define RSA_PUBLIC_HEX  "080012a60430820222300d06092a864886f70d01010105000382020f003082020a0282020100e1beab071d08200bde24eef00d049449b07770ff9910257b2d7d5dda242ce8f0e2f12e1af4b32d9efd2c090f66b0f29986dbb645dae9880089704a94e5066d594162ae6ee8892e6ec70701db0a6c445c04778eb3de1293aa1a23c3825b85c6620a2bc3f82f9b0c309bc0ab3aeb1873282bebd3da03c33e76c21e9beb172fd44c9e43be32e2c99827033cf8d0f0c606f4579326c930eb4e854395ad941256542c793902185153c474bed109d6ff5141ebf9cd256cf58893a37f83729f97e7cb435ec679d2e33901d27bb35aa0d7e20561da08885ef0abbf8e2fb48d6a5487047a9ecb1ad41fa7ed84f6e3e8ecd5d98b3982d2a901b4454991766da295ab78822add5612a2df83bcee814cf50973e80d7ef38111b1bd87da2ae92438a2c8cbcc70b31ee319939a3b9c761dbc13b5c086d6b64bf7ae7dacc14622375d92a8ff9af7eb962162bbddebf90acb32adb5e4e4029f1c96019949ecfbfeffd7ac1e3fbcc6b6168c34be3d5a2e5999fcbb39bba7adbca78eab09b9bc39f7fa4b93411f4cc175e70c0a083e96bfaefb04a9580b4753c1738a6a760ae1afd851a1a4bdad231cf56e9284d832483df215a46c1c21bdf0c6cfe951c18f1ee4078c79c13d63edb6e14feaeffabc90ad317e4875fe648101b0864097e998f0ca3025ef9638cd2b0caecd3770ab54a1d9c6ca959b0f5dcbc90caeefc4135baca6fd475224269bbe1b0203010001"

int main(void)
{
    int failures = 0;
    peer_id_error_t err;
    int ret;

    /***********************
     * Test 1: Create peer ID from a secp256k1 public key
     ***********************/
    {
        size_t pubkey_len = 0;
        uint8_t *pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes\n");
            exit(EXIT_FAILURE);
        }

        peer_id_t pid_pub = {0};
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid_pub);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key()", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key()", "", 1);
        }
        free(pubkey);

        /* Test peer_id_to_string for legacy (base58btc) format. */
        char legacy_str[256];
        ret = peer_id_to_string(&pid_pub, PEER_ID_FMT_BASE58_LEGACY, legacy_str, sizeof(legacy_str));
        if (ret < 0)
        {
            print_standard("peer_id_to_string(legacy)", "Encoding failed", 0);
            failures++;
        }
        else
        {
            char details[256];
            sprintf(details, "Legacy string: %s", legacy_str);
            print_standard("peer_id_to_string(legacy)", details, 1);
        }

        /* Test peer_id_to_string for CIDv1 multibase format. */
        char cid_str[256];
        ret = peer_id_to_string(&pid_pub, PEER_ID_FMT_MULTIBASE_CIDv1, cid_str, sizeof(cid_str));
        if (ret < 0)
        {
            print_standard("peer_id_to_string(CIDv1)", "Encoding failed", 0);
            failures++;
        }
        else
        {
            char details[256];
            sprintf(details, "CIDv1 string: %s", cid_str);
            print_standard("peer_id_to_string(CIDv1)", details, 1);
        }

        peer_id_destroy(&pid_pub);
    }

    /***********************
     * Test 2: Create peer ID from a secp256k1 private key
     ***********************/
    {
        size_t privkey_len = 0;
        uint8_t *privkey = hex_to_bytes(SECP256K1_PRIVATE_HEX, &privkey_len);
        if (!privkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 private hex to bytes\n");
            exit(EXIT_FAILURE);
        }

        peer_id_t pid_priv = {0};
        err = peer_id_create_from_private_key(privkey, privkey_len, &pid_priv);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_private_key()", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_private_key()", "", 1);
        }
        free(privkey);

        /* Also create peer ID from public key and compare. */
        size_t pubkey_len = 0;
        uint8_t *pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid_pub = {0};
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid_pub);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key() (for private key test)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key() (for private key test)", "", 1);
        }
        free(pubkey);

        int eq = peer_id_equals(&pid_priv, &pid_pub);
        if (eq != 1)
        {
            print_standard("peer_id_equals(private vs public)", "Peer IDs do not match", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(private vs public)", "", 1);
        }
        peer_id_destroy(&pid_priv);
        peer_id_destroy(&pid_pub);
    }

    /***********************
     * Test 3: Create peer ID from string representation (secp256k1)
     ***********************/
    {
        /* Legacy (raw base58btc) test */
        size_t pubkey_len = 0;
        uint8_t *pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid = {0};
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid);
        free(pubkey);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key() (for string test)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key() (for string test)", "", 1);
        }

        char legacy_str[256];
        ret = peer_id_to_string(&pid, PEER_ID_FMT_BASE58_LEGACY, legacy_str, sizeof(legacy_str));
        if (ret < 0)
        {
            print_standard("peer_id_to_string(legacy) (for string test)", "Encoding failed", 0);
            failures++;
        }
        else
        {
            char details[256];
            sprintf(details, "Legacy string: %s", legacy_str);
            print_standard("peer_id_to_string(legacy) (for string test)", details, 1);
        }

        peer_id_t pid_from_str = {0};
        err = peer_id_create_from_string(legacy_str, &pid_from_str);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_string(legacy)", "Decoding failed", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_string(legacy)", "", 1);
        }

        if (peer_id_equals(&pid, &pid_from_str) != 1)
        {
            print_standard("peer_id_equals(legacy)", "Decoded peer ID does not match original", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(legacy)", "", 1);
        }
        peer_id_destroy(&pid);
        peer_id_destroy(&pid_from_str);

        /* CIDv1 (multibase) test */
        pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes (CIDv1 test)\n");
            exit(EXIT_FAILURE);
        }
        pid.bytes = NULL;
        pid.size = 0;
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid);
        free(pubkey);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key() (CIDv1 test)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key() (CIDv1 test)", "", 1);
        }

        char cid_str[256];
        ret = peer_id_to_string(&pid, PEER_ID_FMT_MULTIBASE_CIDv1, cid_str, sizeof(cid_str));
        if (ret < 0)
        {
            print_standard("peer_id_to_string(CIDv1) (for string test)", "Encoding failed", 0);
            failures++;
        }
        else
        {
            char details[256];
            sprintf(details, "CIDv1 string: %s", cid_str);
            print_standard("peer_id_to_string(CIDv1) (for string test)", details, 1);
        }

        peer_id_t pid_from_cid = {0};
        err = peer_id_create_from_string(cid_str, &pid_from_cid);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_string(CIDv1)", "Decoding failed", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_string(CIDv1)", "", 1);
        }

        if (peer_id_equals(&pid, &pid_from_cid) != 1)
        {
            print_standard("peer_id_equals(CIDv1)", "Decoded CIDv1 peer ID does not match original", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(CIDv1)", "", 1);
        }
        peer_id_destroy(&pid);
        peer_id_destroy(&pid_from_cid);
    }

    /***********************
     * Test 4: Create and compare RSA peer IDs (public and private)
     ***********************/
    {
        /* RSA public key test */
        size_t pubkey_len = 0;
        uint8_t *rsa_pubkey = hex_to_bytes(RSA_PUBLIC_HEX, &pubkey_len);
        if (!rsa_pubkey)
        {
            fprintf(stderr, "Failed to convert RSA public hex to bytes\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid_rsa_pub = {0};
        err = peer_id_create_from_public_key(rsa_pubkey, pubkey_len, &pid_rsa_pub);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key() (RSA)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key() (RSA)", "", 1);
        }
        free(rsa_pubkey);

        char legacy_str[256];
        ret = peer_id_to_string(&pid_rsa_pub, PEER_ID_FMT_BASE58_LEGACY, legacy_str, sizeof(legacy_str));
        if (ret < 0)
        {
            print_standard("peer_id_to_string(legacy) (RSA)", "Encoding failed", 0);
            failures++;
        }
        else
        {
            char details[256];
            sprintf(details, "RSA Legacy string: %s", legacy_str);
            print_standard("peer_id_to_string(legacy) (RSA)", details, 1);
        }
        peer_id_destroy(&pid_rsa_pub);
    }

    {
        /* RSA private key test: generate peer ID from RSA private key and compare
           to the one derived from the corresponding RSA public key */
        size_t privkey_len = 0;
        uint8_t *rsa_privkey = hex_to_bytes(RSA_PRIVATE_HEX, &privkey_len);
        if (!rsa_privkey)
        {
            fprintf(stderr, "Failed to convert RSA private hex to bytes\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid_rsa_priv = {0};
        err = peer_id_create_from_private_key(rsa_privkey, privkey_len, &pid_rsa_priv);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_private_key() (RSA)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_private_key() (RSA)", "", 1);
        }
        free(rsa_privkey);

        size_t pubkey_len = 0;
        uint8_t *rsa_pubkey = hex_to_bytes(RSA_PUBLIC_HEX, &pubkey_len);
        if (!rsa_pubkey)
        {
            fprintf(stderr, "Failed to convert RSA public hex to bytes (for RSA private test)\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid_rsa_pub = {0};
        err = peer_id_create_from_public_key(rsa_pubkey, pubkey_len, &pid_rsa_pub);
        if (err != PEER_ID_SUCCESS)
        {
            print_standard("peer_id_create_from_public_key() (RSA for private test)", "Returned error", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_create_from_public_key() (RSA for private test)", "", 1);
        }
        free(rsa_pubkey);

        int eq = peer_id_equals(&pid_rsa_priv, &pid_rsa_pub);
        if (eq != 1)
        {
            print_standard("peer_id_equals(RSA private vs public)", "Peer IDs do not match", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(RSA private vs public)", "", 1);
        }
        peer_id_destroy(&pid_rsa_priv);
        peer_id_destroy(&pid_rsa_pub);
    }

    /***********************
     * Test 5: Test peer_id_equals with different peer IDs (secp256k1)
     ***********************/
    {
        size_t pubkey_len = 0;
        uint8_t *pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes (equals test)\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid1 = {0}, pid2 = {0};
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid1);
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid2);
        free(pubkey);

        if (peer_id_equals(&pid1, &pid2) != 1)
        {
            print_standard("peer_id_equals(same IDs)", "Peer IDs should be equal", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(same IDs)", "", 1);
        }

        /* Modify one byte in pid2 and ensure they are no longer equal */
        if (pid2.size > 0)
        {
            pid2.bytes[0] ^= 0xFF;
        }
        if (peer_id_equals(&pid1, &pid2) != 0)
        {
            print_standard("peer_id_equals(different IDs)", "Peer IDs should not be equal", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_equals(different IDs)", "", 1);
        }
        peer_id_destroy(&pid1);
        peer_id_destroy(&pid2);
    }

    /***********************
     * Test 6: Test peer_id_destroy
     ***********************/
    {
        size_t pubkey_len = 0;
        uint8_t *pubkey = hex_to_bytes(SECP256K1_PUBLIC_HEX, &pubkey_len);
        if (!pubkey)
        {
            fprintf(stderr, "Failed to convert secp256k1 public hex to bytes (destroy test)\n");
            exit(EXIT_FAILURE);
        }
        peer_id_t pid = {0};
        err = peer_id_create_from_public_key(pubkey, pubkey_len, &pid);
        free(pubkey);
        peer_id_destroy(&pid);
        if (pid.bytes != NULL || pid.size != 0)
        {
            print_standard("peer_id_destroy()", "Peer ID not properly destroyed", 0);
            failures++;
        }
        else
        {
            print_standard("peer_id_destroy()", "", 1);
        }
    }

    if (failures)
    {
        printf("\nSome tests failed. Total failures: %d\n", failures);
        return EXIT_FAILURE;
    }
    else
    {
        printf("\nAll tests passed!\n");
        return EXIT_SUCCESS;
    }
}