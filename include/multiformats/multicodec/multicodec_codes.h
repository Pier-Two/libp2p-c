#ifndef MULTICODEC_CODES_H
#define MULTICODEC_CODES_H

/*
 * This header provides macros for each name→code mapping in the
 * multicodec table. It ignores status, tag, and description fields.
 *
 * Reference: https://github.com/multiformats/multicodec
 */

/* ============== 0x00 - 0xFF Range (Single-Byte Codes) ============== */
#define MULTICODEC_IDENTITY 0x00
#define MULTICODEC_CIDV1 0x01
#define MULTICODEC_CIDV2 0x02
#define MULTICODEC_CIDV3 0x03
#define MULTICODEC_IP4 0x04
#define MULTICODEC_TCP 0x06
#define MULTICODEC_SHA1 0x11
#define MULTICODEC_SHA2_256 0x12
#define MULTICODEC_SHA2_512 0x13
#define MULTICODEC_SHA3_512 0x14
#define MULTICODEC_SHA3_384 0x15
#define MULTICODEC_SHA3_256 0x16
#define MULTICODEC_SHA3_224 0x17
#define MULTICODEC_SHAKE_128 0x18
#define MULTICODEC_SHAKE_256 0x19
#define MULTICODEC_KECCAK_224 0x1a
#define MULTICODEC_KECCAK_256 0x1b
#define MULTICODEC_KECCAK_384 0x1c
#define MULTICODEC_KECCAK_512 0x1d
#define MULTICODEC_BLAKE3 0x1e
#define MULTICODEC_SHA2_384 0x20
#define MULTICODEC_DCCP 0x21
#define MULTICODEC_MURMUR3_X64_64 0x22
#define MULTICODEC_MURMUR3_32 0x23
#define MULTICODEC_IP6 0x29
#define MULTICODEC_IP6ZONE 0x2a
#define MULTICODEC_IPCIDR 0x2b
#define MULTICODEC_PATH 0x2f
#define MULTICODEC_MULTICODEC 0x30
#define MULTICODEC_MULTIHASH 0x31
#define MULTICODEC_MULTIADDR 0x32
#define MULTICODEC_MULTIBASE 0x33
#define MULTICODEC_VARSIG 0x34
#define MULTICODEC_DNS 0x35
#define MULTICODEC_DNS4 0x36
#define MULTICODEC_DNS6 0x37
#define MULTICODEC_DNSADDR 0x38
#define MULTICODEC_PROTOBUF 0x50
#define MULTICODEC_CBOR 0x51
#define MULTICODEC_RAW 0x55
#define MULTICODEC_DBL_SHA2_256 0x56
#define MULTICODEC_RLP 0x60
#define MULTICODEC_BENCODE 0x63
#define MULTICODEC_DAG_PB 0x70
#define MULTICODEC_DAG_CBOR 0x71
#define MULTICODEC_LIBP2P_KEY 0x72
#define MULTICODEC_GIT_RAW 0x78
#define MULTICODEC_TORRENT_INFO 0x7b
#define MULTICODEC_TORRENT_FILE 0x7c
#define MULTICODEC_BLAKE3_HASHSEQ 0x80
#define MULTICODEC_LEOFCOIN_BLOCK 0x81
#define MULTICODEC_LEOFCOIN_TX 0x82
#define MULTICODEC_LEOFCOIN_PR 0x83
#define MULTICODEC_SCTP 0x84
#define MULTICODEC_DAG_JOSE 0x85
#define MULTICODEC_DAG_COSE 0x86
#define MULTICODEC_LBRY 0x8c
#define MULTICODEC_ETH_BLOCK 0x90
#define MULTICODEC_ETH_BLOCK_LIST 0x91
#define MULTICODEC_ETH_TX_TRIE 0x92
#define MULTICODEC_ETH_TX 0x93
#define MULTICODEC_ETH_TX_RECEIPT_TRIE 0x94
#define MULTICODEC_ETH_TX_RECEIPT 0x95
#define MULTICODEC_ETH_STATE_TRIE 0x96
#define MULTICODEC_ETH_ACCOUNT_SNAPSHOT 0x97
#define MULTICODEC_ETH_STORAGE_TRIE 0x98
#define MULTICODEC_ETH_RECEIPT_LOG_TRIE 0x99
#define MULTICODEC_ETH_RECEIPT_LOG 0x9a
#define MULTICODEC_AES_128 0xa0
#define MULTICODEC_AES_192 0xa1
#define MULTICODEC_AES_256 0xa2
#define MULTICODEC_CHACHA_128 0xa3
#define MULTICODEC_CHACHA_256 0xa4
#define MULTICODEC_BITCOIN_BLOCK 0xb0
#define MULTICODEC_BITCOIN_TX 0xb1
#define MULTICODEC_BITCOIN_WITNESS_COMMITMENT 0xb2
#define MULTICODEC_ZCASH_BLOCK 0xc0
#define MULTICODEC_ZCASH_TX 0xc1
#define MULTICODEC_CAIP_50 0xca
#define MULTICODEC_STREAMID 0xce
#define MULTICODEC_STELLAR_BLOCK 0xd0
#define MULTICODEC_STELLAR_TX 0xd1
#define MULTICODEC_MD4 0xd4
#define MULTICODEC_MD5 0xd5
#define MULTICODEC_DECRED_BLOCK 0xe0
#define MULTICODEC_DECRED_TX 0xe1
#define MULTICODEC_IPLD 0xe2
#define MULTICODEC_IPFS 0xe3
#define MULTICODEC_SWARM 0xe4
#define MULTICODEC_IPNS 0xe5
#define MULTICODEC_ZERONET 0xe6
#define MULTICODEC_SECP256K1_PUB 0xe7
#define MULTICODEC_DNSLINK 0xe8
#define MULTICODEC_BLS12_381_G1_PUB 0xea
#define MULTICODEC_BLS12_381_G2_PUB 0xeb
#define MULTICODEC_X25519_PUB 0xec
#define MULTICODEC_ED25519_PUB 0xed
#define MULTICODEC_BLS12_381_G1G2_PUB 0xee
#define MULTICODEC_SR25519_PUB 0xef
#define MULTICODEC_DASH_BLOCK 0xf0
#define MULTICODEC_DASH_TX 0xf1
#define MULTICODEC_SWARM_MANIFEST 0xfa
#define MULTICODEC_SWARM_FEED 0xfb
#define MULTICODEC_BEESON 0xfc

/* ============== 0x0100 - 0x01FF Range ============== */
#define MULTICODEC_UDP 0x0111
#define MULTICODEC_P2P_WEBRTC_STAR 0x0113   /* deprecated */
#define MULTICODEC_P2P_WEBRTC_DIRECT 0x0114 /* deprecated */
#define MULTICODEC_P2P_STARDUST 0x0115      /* deprecated */
#define MULTICODEC_WEBRTC_DIRECT 0x0118
#define MULTICODEC_WEBRTC 0x0119
#define MULTICODEC_P2P_CIRCUIT 0x0122
#define MULTICODEC_DAG_JSON 0x0129
#define MULTICODEC_UDT 0x012d
#define MULTICODEC_UTP 0x012e
#define MULTICODEC_CRC32 0x0132
#define MULTICODEC_CRC64_ECMA 0x0164
#define MULTICODEC_UNIX 0x0190
#define MULTICODEC_THREAD 0x0196
#define MULTICODEC_P2P 0x01a5
#define MULTICODEC_HTTPS 0x01bb
#define MULTICODEC_ONION 0x01bc
#define MULTICODEC_ONION3 0x01bd
#define MULTICODEC_GARLIC64 0x01be
#define MULTICODEC_GARLIC32 0x01bf
#define MULTICODEC_TLS 0x01c0
#define MULTICODEC_SNI 0x01c1
#define MULTICODEC_NOISE 0x01c6
#define MULTICODEC_SHS 0x01c8
#define MULTICODEC_QUIC 0x01cc
#define MULTICODEC_QUIC_V1 0x01cd
#define MULTICODEC_WEBTRANSPORT 0x01d1
#define MULTICODEC_CERTHASH 0x01d2
#define MULTICODEC_WS 0x01dd
#define MULTICODEC_WSS 0x01de
#define MULTICODEC_P2P_WEBSOCKET_STAR 0x01df
#define MULTICODEC_HTTP 0x01e0
#define MULTICODEC_HTTP_PATH 0x01e1
#define MULTICODEC_SWHID_1_SNP 0x01f0

/* ============== 0x0200 - 0x02FF Range ============== */
#define MULTICODEC_JSON 0x0200
#define MULTICODEC_MESSAGEPACK 0x0201
#define MULTICODEC_CAR 0x0202

/* ============== 0x0300 - 0x03FF Range ============== */
#define MULTICODEC_IPNS_RECORD 0x0300
#define MULTICODEC_LIBP2P_PEER_RECORD 0x0301
#define MULTICODEC_LIBP2P_RELAY_RSVP 0x0302
#define MULTICODEC_MEMORYTRANSPORT 0x0309

/* ============== 0x0400 - 0x04FF Range ============== */
#define MULTICODEC_CAR_INDEX_SORTED 0x0400
#define MULTICODEC_CAR_MULTIHASH_INDEX_SORTED 0x0401

/* ============== 0x0900 - 0x09FF Range ============== */
#define MULTICODEC_TRANSPORT_BITSWAP 0x0900
#define MULTICODEC_TRANSPORT_GRAPHSYNC_FILECOINV1 0x0910
#define MULTICODEC_TRANSPORT_IPFS_GATEWAY_HTTP 0x0920

/* ============== 0x0D00 - 0x0DFF Range ============== */
#define MULTICODEC_MULTIDID 0x0d1d

/* ============== 0x1000 - 0x10FF Range ============== */
#define MULTICODEC_SHA2_256_TRUNC254_PADDED 0x1012
#define MULTICODEC_SHA2_224 0x1013
#define MULTICODEC_SHA2_512_224 0x1014
#define MULTICODEC_SHA2_512_256 0x1015
#define MULTICODEC_MURMUR3_X64_128 0x1022
#define MULTICODEC_RIPEMD_128 0x1052
#define MULTICODEC_RIPEMD_160 0x1053
#define MULTICODEC_RIPEMD_256 0x1054
#define MULTICODEC_RIPEMD_320 0x1055
#define MULTICODEC_X11 0x1100

/* ============== 0x1200 - 0x12FF Range (key + misc) ============== */
#define MULTICODEC_P256_PUB 0x1200
#define MULTICODEC_P384_PUB 0x1201
#define MULTICODEC_P521_PUB 0x1202
#define MULTICODEC_ED448_PUB 0x1203
#define MULTICODEC_X448_PUB 0x1204
#define MULTICODEC_RSA_PUB 0x1205
#define MULTICODEC_SM2_PUB 0x1206
#define MULTICODEC_VLAD 0x1207
#define MULTICODEC_PROVENANCE_LOG 0x1208
#define MULTICODEC_PROVENANCE_LOG_ENTRY 0x1209
#define MULTICODEC_PROVENANCE_LOG_SCRIPT 0x120a
#define MULTICODEC_MLKEM_512_PUB 0x120b
#define MULTICODEC_MLKEM_768_PUB 0x120c
#define MULTICODEC_MLKEM_1024_PUB 0x120d
#define MULTICODEC_MULTISIG 0x1239
#define MULTICODEC_MULTIKEY 0x123a
#define MULTICODEC_NONCE 0x123b

/* ============== 0x1300 - 0x13FF Range (private keys, shares, etc.) ============== */
#define MULTICODEC_ED25519_PRIV 0x1300
#define MULTICODEC_SECP256K1_PRIV 0x1301
#define MULTICODEC_X25519_PRIV 0x1302
#define MULTICODEC_SR25519_PRIV 0x1303
#define MULTICODEC_RSA_PRIV 0x1305
#define MULTICODEC_P256_PRIV 0x1306
#define MULTICODEC_P384_PRIV 0x1307
#define MULTICODEC_P521_PRIV 0x1308
#define MULTICODEC_BLS12_381_G1_PRIV 0x1309
#define MULTICODEC_BLS12_381_G2_PRIV 0x130a
#define MULTICODEC_BLS12_381_G1G2_PRIV 0x130b
#define MULTICODEC_BLS12_381_G1_PUB_SHARE 0x130c
#define MULTICODEC_BLS12_381_G2_PUB_SHARE 0x130d
#define MULTICODEC_BLS12_381_G1_PRIV_SHARE 0x130e
#define MULTICODEC_BLS12_381_G2_PRIV_SHARE 0x130f
#define MULTICODEC_SM2_PRIV 0x1310

/* ============== 0x1A00 - 0x1AFF Range (Lamport, etc.) ============== */
#define MULTICODEC_LAMPORT_SHA3_512_PUB 0x1a14
#define MULTICODEC_LAMPORT_SHA3_384_PUB 0x1a15
#define MULTICODEC_LAMPORT_SHA3_256_PUB 0x1a16
#define MULTICODEC_LAMPORT_SHA3_512_PRIV 0x1a24
#define MULTICODEC_LAMPORT_SHA3_384_PRIV 0x1a25
#define MULTICODEC_LAMPORT_SHA3_256_PRIV 0x1a26
#define MULTICODEC_LAMPORT_SHA3_512_PRIV_SHARE 0x1a34
#define MULTICODEC_LAMPORT_SHA3_384_PRIV_SHARE 0x1a35
#define MULTICODEC_LAMPORT_SHA3_256_PRIV_SHARE 0x1a36
#define MULTICODEC_LAMPORT_SHA3_512_SIG 0x1a44
#define MULTICODEC_LAMPORT_SHA3_384_SIG 0x1a45
#define MULTICODEC_LAMPORT_SHA3_256_SIG 0x1a46
#define MULTICODEC_LAMPORT_SHA3_512_SIG_SHARE 0x1a54
#define MULTICODEC_LAMPORT_SHA3_384_SIG_SHARE 0x1a55
#define MULTICODEC_LAMPORT_SHA3_256_SIG_SHARE 0x1a56

/* KangarooTwelve */
#define MULTICODEC_KANGAROOTWELVE 0x1d01

/* ============== 0x2000 Range (Encryption) ============== */
#define MULTICODEC_AES_GCM_256 0x2000

/* ============== 0x3F00 - 0x3FFF Range ============== */
#define MULTICODEC_SILVERPINE 0x3f42

/* ============== 0x534D Range (SM3) ============== */
#define MULTICODEC_SM3_256 0x534d

/* ============== 0x7012 Range (Ceramic CIP-124) ============== */
#define MULTICODEC_SHA256A 0x7012

/* ============== 0xA000 Range (ChaCha20_Poly1305) ============== */
#define MULTICODEC_CHACHA20_POLY1305 0xa000

/* ============== 0xB200 - 0xB3FF Range (Blake2, Skein, etc.) ============== */
/* --- BLAKE2b --- */
#define MULTICODEC_BLAKE2B_8 0xb201
#define MULTICODEC_BLAKE2B_16 0xb202
#define MULTICODEC_BLAKE2B_24 0xb203
#define MULTICODEC_BLAKE2B_32 0xb204
#define MULTICODEC_BLAKE2B_40 0xb205
#define MULTICODEC_BLAKE2B_48 0xb206
#define MULTICODEC_BLAKE2B_56 0xb207
#define MULTICODEC_BLAKE2B_64 0xb208
#define MULTICODEC_BLAKE2B_72 0xb209
#define MULTICODEC_BLAKE2B_80 0xb20a
#define MULTICODEC_BLAKE2B_88 0xb20b
#define MULTICODEC_BLAKE2B_96 0xb20c
#define MULTICODEC_BLAKE2B_104 0xb20d
#define MULTICODEC_BLAKE2B_112 0xb20e
#define MULTICODEC_BLAKE2B_120 0xb20f
#define MULTICODEC_BLAKE2B_128 0xb210
#define MULTICODEC_BLAKE2B_136 0xb211
#define MULTICODEC_BLAKE2B_144 0xb212
#define MULTICODEC_BLAKE2B_152 0xb213
#define MULTICODEC_BLAKE2B_160 0xb214
#define MULTICODEC_BLAKE2B_168 0xb215
#define MULTICODEC_BLAKE2B_176 0xb216
#define MULTICODEC_BLAKE2B_184 0xb217
#define MULTICODEC_BLAKE2B_192 0xb218
#define MULTICODEC_BLAKE2B_200 0xb219
#define MULTICODEC_BLAKE2B_208 0xb21a
#define MULTICODEC_BLAKE2B_216 0xb21b
#define MULTICODEC_BLAKE2B_224 0xb21c
#define MULTICODEC_BLAKE2B_232 0xb21d
#define MULTICODEC_BLAKE2B_240 0xb21e
#define MULTICODEC_BLAKE2B_248 0xb21f
#define MULTICODEC_BLAKE2B_256 0xb220
#define MULTICODEC_BLAKE2B_264 0xb221
#define MULTICODEC_BLAKE2B_272 0xb222
#define MULTICODEC_BLAKE2B_280 0xb223
#define MULTICODEC_BLAKE2B_288 0xb224
#define MULTICODEC_BLAKE2B_296 0xb225
#define MULTICODEC_BLAKE2B_304 0xb226
#define MULTICODEC_BLAKE2B_312 0xb227
#define MULTICODEC_BLAKE2B_320 0xb228
#define MULTICODEC_BLAKE2B_328 0xb229
#define MULTICODEC_BLAKE2B_336 0xb22a
#define MULTICODEC_BLAKE2B_344 0xb22b
#define MULTICODEC_BLAKE2B_352 0xb22c
#define MULTICODEC_BLAKE2B_360 0xb22d
#define MULTICODEC_BLAKE2B_368 0xb22e
#define MULTICODEC_BLAKE2B_376 0xb22f
#define MULTICODEC_BLAKE2B_384 0xb230
#define MULTICODEC_BLAKE2B_392 0xb231
#define MULTICODEC_BLAKE2B_400 0xb232
#define MULTICODEC_BLAKE2B_408 0xb233
#define MULTICODEC_BLAKE2B_416 0xb234
#define MULTICODEC_BLAKE2B_424 0xb235
#define MULTICODEC_BLAKE2B_432 0xb236
#define MULTICODEC_BLAKE2B_440 0xb237
#define MULTICODEC_BLAKE2B_448 0xb238
#define MULTICODEC_BLAKE2B_456 0xb239
#define MULTICODEC_BLAKE2B_464 0xb23a
#define MULTICODEC_BLAKE2B_472 0xb23b
#define MULTICODEC_BLAKE2B_480 0xb23c
#define MULTICODEC_BLAKE2B_488 0xb23d
#define MULTICODEC_BLAKE2B_496 0xb23e
#define MULTICODEC_BLAKE2B_504 0xb23f
#define MULTICODEC_BLAKE2B_512 0xb240

/* --- BLAKE2s --- */
#define MULTICODEC_BLAKE2S_8 0xb241
#define MULTICODEC_BLAKE2S_16 0xb242
#define MULTICODEC_BLAKE2S_24 0xb243
#define MULTICODEC_BLAKE2S_32 0xb244
#define MULTICODEC_BLAKE2S_40 0xb245
#define MULTICODEC_BLAKE2S_48 0xb246
#define MULTICODEC_BLAKE2S_56 0xb247
#define MULTICODEC_BLAKE2S_64 0xb248
#define MULTICODEC_BLAKE2S_72 0xb249
#define MULTICODEC_BLAKE2S_80 0xb24a
#define MULTICODEC_BLAKE2S_88 0xb24b
#define MULTICODEC_BLAKE2S_96 0xb24c
#define MULTICODEC_BLAKE2S_104 0xb24d
#define MULTICODEC_BLAKE2S_112 0xb24e
#define MULTICODEC_BLAKE2S_120 0xb24f
#define MULTICODEC_BLAKE2S_128 0xb250
#define MULTICODEC_BLAKE2S_136 0xb251
#define MULTICODEC_BLAKE2S_144 0xb252
#define MULTICODEC_BLAKE2S_152 0xb253
#define MULTICODEC_BLAKE2S_160 0xb254
#define MULTICODEC_BLAKE2S_168 0xb255
#define MULTICODEC_BLAKE2S_176 0xb256
#define MULTICODEC_BLAKE2S_184 0xb257
#define MULTICODEC_BLAKE2S_192 0xb258
#define MULTICODEC_BLAKE2S_200 0xb259
#define MULTICODEC_BLAKE2S_208 0xb25a
#define MULTICODEC_BLAKE2S_216 0xb25b
#define MULTICODEC_BLAKE2S_224 0xb25c
#define MULTICODEC_BLAKE2S_232 0xb25d
#define MULTICODEC_BLAKE2S_240 0xb25e
#define MULTICODEC_BLAKE2S_248 0xb25f
#define MULTICODEC_BLAKE2S_256 0xb260

/* --- Skein (256-bit) --- */
#define MULTICODEC_SKEIN256_8 0xb301
#define MULTICODEC_SKEIN256_16 0xb302
#define MULTICODEC_SKEIN256_24 0xb303
#define MULTICODEC_SKEIN256_32 0xb304
#define MULTICODEC_SKEIN256_40 0xb305
#define MULTICODEC_SKEIN256_48 0xb306
#define MULTICODEC_SKEIN256_56 0xb307
#define MULTICODEC_SKEIN256_64 0xb308
#define MULTICODEC_SKEIN256_72 0xb309
#define MULTICODEC_SKEIN256_80 0xb30a
#define MULTICODEC_SKEIN256_88 0xb30b
#define MULTICODEC_SKEIN256_96 0xb30c
#define MULTICODEC_SKEIN256_104 0xb30d
#define MULTICODEC_SKEIN256_112 0xb30e
#define MULTICODEC_SKEIN256_120 0xb30f
#define MULTICODEC_SKEIN256_128 0xb310
#define MULTICODEC_SKEIN256_136 0xb311
#define MULTICODEC_SKEIN256_144 0xb312
#define MULTICODEC_SKEIN256_152 0xb313
#define MULTICODEC_SKEIN256_160 0xb314
#define MULTICODEC_SKEIN256_168 0xb315
#define MULTICODEC_SKEIN256_176 0xb316
#define MULTICODEC_SKEIN256_184 0xb317
#define MULTICODEC_SKEIN256_192 0xb318
#define MULTICODEC_SKEIN256_200 0xb319
#define MULTICODEC_SKEIN256_208 0xb31a
#define MULTICODEC_SKEIN256_216 0xb31b
#define MULTICODEC_SKEIN256_224 0xb31c
#define MULTICODEC_SKEIN256_232 0xb31d
#define MULTICODEC_SKEIN256_240 0xb31e
#define MULTICODEC_SKEIN256_248 0xb31f
#define MULTICODEC_SKEIN256_256 0xb320

/* --- Skein (512-bit) --- */
#define MULTICODEC_SKEIN512_8 0xb321
#define MULTICODEC_SKEIN512_16 0xb322
#define MULTICODEC_SKEIN512_24 0xb323
#define MULTICODEC_SKEIN512_32 0xb324
#define MULTICODEC_SKEIN512_40 0xb325
#define MULTICODEC_SKEIN512_48 0xb326
#define MULTICODEC_SKEIN512_56 0xb327
#define MULTICODEC_SKEIN512_64 0xb328
#define MULTICODEC_SKEIN512_72 0xb329
#define MULTICODEC_SKEIN512_80 0xb32a
#define MULTICODEC_SKEIN512_88 0xb32b
#define MULTICODEC_SKEIN512_96 0xb32c
#define MULTICODEC_SKEIN512_104 0xb32d
#define MULTICODEC_SKEIN512_112 0xb32e
#define MULTICODEC_SKEIN512_120 0xb32f
#define MULTICODEC_SKEIN512_128 0xb330
#define MULTICODEC_SKEIN512_136 0xb331
#define MULTICODEC_SKEIN512_144 0xb332
#define MULTICODEC_SKEIN512_152 0xb333
#define MULTICODEC_SKEIN512_160 0xb334
#define MULTICODEC_SKEIN512_168 0xb335
#define MULTICODEC_SKEIN512_176 0xb336
#define MULTICODEC_SKEIN512_184 0xb337
#define MULTICODEC_SKEIN512_192 0xb338
#define MULTICODEC_SKEIN512_200 0xb339
#define MULTICODEC_SKEIN512_208 0xb33a
#define MULTICODEC_SKEIN512_216 0xb33b
#define MULTICODEC_SKEIN512_224 0xb33c
#define MULTICODEC_SKEIN512_232 0xb33d
#define MULTICODEC_SKEIN512_240 0xb33e
#define MULTICODEC_SKEIN512_248 0xb33f
#define MULTICODEC_SKEIN512_256 0xb340
#define MULTICODEC_SKEIN512_264 0xb341
#define MULTICODEC_SKEIN512_272 0xb342
#define MULTICODEC_SKEIN512_280 0xb343
#define MULTICODEC_SKEIN512_288 0xb344
#define MULTICODEC_SKEIN512_296 0xb345
#define MULTICODEC_SKEIN512_304 0xb346
#define MULTICODEC_SKEIN512_312 0xb347
#define MULTICODEC_SKEIN512_320 0xb348
#define MULTICODEC_SKEIN512_328 0xb349
#define MULTICODEC_SKEIN512_336 0xb34a
#define MULTICODEC_SKEIN512_344 0xb34b
#define MULTICODEC_SKEIN512_352 0xb34c
#define MULTICODEC_SKEIN512_360 0xb34d
#define MULTICODEC_SKEIN512_368 0xb34e
#define MULTICODEC_SKEIN512_376 0xb34f
#define MULTICODEC_SKEIN512_384 0xb350
#define MULTICODEC_SKEIN512_392 0xb351
#define MULTICODEC_SKEIN512_400 0xb352
#define MULTICODEC_SKEIN512_408 0xb353
#define MULTICODEC_SKEIN512_416 0xb354
#define MULTICODEC_SKEIN512_424 0xb355
#define MULTICODEC_SKEIN512_432 0xb356
#define MULTICODEC_SKEIN512_440 0xb357
#define MULTICODEC_SKEIN512_448 0xb358
#define MULTICODEC_SKEIN512_456 0xb359
#define MULTICODEC_SKEIN512_464 0xb35a
#define MULTICODEC_SKEIN512_472 0xb35b
#define MULTICODEC_SKEIN512_480 0xb35c
#define MULTICODEC_SKEIN512_488 0xb35d
#define MULTICODEC_SKEIN512_496 0xb35e
#define MULTICODEC_SKEIN512_504 0xb35f
#define MULTICODEC_SKEIN512_512 0xb360

/* --- Skein (1024-bit) --- */
#define MULTICODEC_SKEIN1024_8 0xb361
#define MULTICODEC_SKEIN1024_16 0xb362
#define MULTICODEC_SKEIN1024_24 0xb363
#define MULTICODEC_SKEIN1024_32 0xb364
#define MULTICODEC_SKEIN1024_40 0xb365
#define MULTICODEC_SKEIN1024_48 0xb366
#define MULTICODEC_SKEIN1024_56 0xb367
#define MULTICODEC_SKEIN1024_64 0xb368
#define MULTICODEC_SKEIN1024_72 0xb369
#define MULTICODEC_SKEIN1024_80 0xb36a
#define MULTICODEC_SKEIN1024_88 0xb36b
#define MULTICODEC_SKEIN1024_96 0xb36c
#define MULTICODEC_SKEIN1024_104 0xb36d
#define MULTICODEC_SKEIN1024_112 0xb36e
#define MULTICODEC_SKEIN1024_120 0xb36f
#define MULTICODEC_SKEIN1024_128 0xb370
#define MULTICODEC_SKEIN1024_136 0xb371
#define MULTICODEC_SKEIN1024_144 0xb372
#define MULTICODEC_SKEIN1024_152 0xb373
#define MULTICODEC_SKEIN1024_160 0xb374
#define MULTICODEC_SKEIN1024_168 0xb375
#define MULTICODEC_SKEIN1024_176 0xb376
#define MULTICODEC_SKEIN1024_184 0xb377
#define MULTICODEC_SKEIN1024_192 0xb378
#define MULTICODEC_SKEIN1024_200 0xb379
#define MULTICODEC_SKEIN1024_208 0xb37a
#define MULTICODEC_SKEIN1024_216 0xb37b
#define MULTICODEC_SKEIN1024_224 0xb37c
#define MULTICODEC_SKEIN1024_232 0xb37d
#define MULTICODEC_SKEIN1024_240 0xb37e
#define MULTICODEC_SKEIN1024_248 0xb37f
#define MULTICODEC_SKEIN1024_256 0xb380
#define MULTICODEC_SKEIN1024_264 0xb381
#define MULTICODEC_SKEIN1024_272 0xb382
#define MULTICODEC_SKEIN1024_280 0xb383
#define MULTICODEC_SKEIN1024_288 0xb384
#define MULTICODEC_SKEIN1024_296 0xb385
#define MULTICODEC_SKEIN1024_304 0xb386
#define MULTICODEC_SKEIN1024_312 0xb387
#define MULTICODEC_SKEIN1024_320 0xb388
#define MULTICODEC_SKEIN1024_328 0xb389
#define MULTICODEC_SKEIN1024_336 0xb38a
#define MULTICODEC_SKEIN1024_344 0xb38b
#define MULTICODEC_SKEIN1024_352 0xb38c
#define MULTICODEC_SKEIN1024_360 0xb38d
#define MULTICODEC_SKEIN1024_368 0xb38e
#define MULTICODEC_SKEIN1024_376 0xb38f
#define MULTICODEC_SKEIN1024_384 0xb390
#define MULTICODEC_SKEIN1024_392 0xb391
#define MULTICODEC_SKEIN1024_400 0xb392
#define MULTICODEC_SKEIN1024_408 0xb393
#define MULTICODEC_SKEIN1024_416 0xb394
#define MULTICODEC_SKEIN1024_424 0xb395
#define MULTICODEC_SKEIN1024_432 0xb396
#define MULTICODEC_SKEIN1024_440 0xb397
#define MULTICODEC_SKEIN1024_448 0xb398
#define MULTICODEC_SKEIN1024_456 0xb399
#define MULTICODEC_SKEIN1024_464 0xb39a
#define MULTICODEC_SKEIN1024_472 0xb39b
#define MULTICODEC_SKEIN1024_480 0xb39c
#define MULTICODEC_SKEIN1024_488 0xb39d
#define MULTICODEC_SKEIN1024_496 0xb39e
#define MULTICODEC_SKEIN1024_504 0xb39f
#define MULTICODEC_SKEIN1024_512 0xb3a0
#define MULTICODEC_SKEIN1024_520 0xb3a1
#define MULTICODEC_SKEIN1024_528 0xb3a2
#define MULTICODEC_SKEIN1024_536 0xb3a3
#define MULTICODEC_SKEIN1024_544 0xb3a4
#define MULTICODEC_SKEIN1024_552 0xb3a5
#define MULTICODEC_SKEIN1024_560 0xb3a6
#define MULTICODEC_SKEIN1024_568 0xb3a7
#define MULTICODEC_SKEIN1024_576 0xb3a8
#define MULTICODEC_SKEIN1024_584 0xb3a9
#define MULTICODEC_SKEIN1024_592 0xb3aa
#define MULTICODEC_SKEIN1024_600 0xb3ab
#define MULTICODEC_SKEIN1024_608 0xb3ac
#define MULTICODEC_SKEIN1024_616 0xb3ad
#define MULTICODEC_SKEIN1024_624 0xb3ae
#define MULTICODEC_SKEIN1024_632 0xb3af
#define MULTICODEC_SKEIN1024_640 0xb3b0
#define MULTICODEC_SKEIN1024_648 0xb3b1
#define MULTICODEC_SKEIN1024_656 0xb3b2
#define MULTICODEC_SKEIN1024_664 0xb3b3
#define MULTICODEC_SKEIN1024_672 0xb3b4
#define MULTICODEC_SKEIN1024_680 0xb3b5
#define MULTICODEC_SKEIN1024_688 0xb3b6
#define MULTICODEC_SKEIN1024_696 0xb3b7
#define MULTICODEC_SKEIN1024_704 0xb3b8
#define MULTICODEC_SKEIN1024_712 0xb3b9
#define MULTICODEC_SKEIN1024_720 0xb3ba
#define MULTICODEC_SKEIN1024_728 0xb3bb
#define MULTICODEC_SKEIN1024_736 0xb3bc
#define MULTICODEC_SKEIN1024_744 0xb3bd
#define MULTICODEC_SKEIN1024_752 0xb3be
#define MULTICODEC_SKEIN1024_760 0xb3bf
#define MULTICODEC_SKEIN1024_768 0xb3c0
#define MULTICODEC_SKEIN1024_776 0xb3c1
#define MULTICODEC_SKEIN1024_784 0xb3c2
#define MULTICODEC_SKEIN1024_792 0xb3c3
#define MULTICODEC_SKEIN1024_800 0xb3c4
#define MULTICODEC_SKEIN1024_808 0xb3c5
#define MULTICODEC_SKEIN1024_816 0xb3c6
#define MULTICODEC_SKEIN1024_824 0xb3c7
#define MULTICODEC_SKEIN1024_832 0xb3c8
#define MULTICODEC_SKEIN1024_840 0xb3c9
#define MULTICODEC_SKEIN1024_848 0xb3ca
#define MULTICODEC_SKEIN1024_856 0xb3cb
#define MULTICODEC_SKEIN1024_864 0xb3cc
#define MULTICODEC_SKEIN1024_872 0xb3cd
#define MULTICODEC_SKEIN1024_880 0xb3ce
#define MULTICODEC_SKEIN1024_888 0xb3cf
#define MULTICODEC_SKEIN1024_896 0xb3d0
#define MULTICODEC_SKEIN1024_904 0xb3d1
#define MULTICODEC_SKEIN1024_912 0xb3d2
#define MULTICODEC_SKEIN1024_920 0xb3d3
#define MULTICODEC_SKEIN1024_928 0xb3d4
#define MULTICODEC_SKEIN1024_936 0xb3d5
#define MULTICODEC_SKEIN1024_944 0xb3d6
#define MULTICODEC_SKEIN1024_952 0xb3d7
#define MULTICODEC_SKEIN1024_960 0xb3d8
#define MULTICODEC_SKEIN1024_968 0xb3d9
#define MULTICODEC_SKEIN1024_976 0xb3da
#define MULTICODEC_SKEIN1024_984 0xb3db
#define MULTICODEC_SKEIN1024_992 0xb3dc
#define MULTICODEC_SKEIN1024_1000 0xb3dd
#define MULTICODEC_SKEIN1024_1008 0xb3de
#define MULTICODEC_SKEIN1024_1016 0xb3df
#define MULTICODEC_SKEIN1024_1024 0xb3e0

/* ============== XXH Hashes ============== */
#define MULTICODEC_XXH_32 0xb3e1
#define MULTICODEC_XXH_64 0xb3e2
#define MULTICODEC_XXH3_64 0xb3e3
#define MULTICODEC_XXH3_128 0xb3e4

/* ============== Poseidon, RDF, SSZ ============== */
#define MULTICODEC_POSEIDON_BLS12_381_A2_FC1 0xb401
#define MULTICODEC_POSEIDON_BLS12_381_A2_FC1_SC 0xb402
#define MULTICODEC_RDFC_1 0xb403
#define MULTICODEC_SSZ 0xb501
#define MULTICODEC_SSZ_SHA2_256_BMT 0xb502
#define MULTICODEC_SHA2_256_CHUNKED 0xb510
#define MULTICODEC_JSON_JCS 0xb601

/* ============== 0xCC00 Range ============== */
#define MULTICODEC_ISCC 0xcc01

/* ============== 0xCE00 Range ============== */
#define MULTICODEC_ZEROXCERT_IMPRINT_256 0xce11

/* ============== 0xD000 - 0xDFFF Range (varsig, etc.) ============== */
#define MULTICODEC_NONSTANDARD_SIG 0xd000
#define MULTICODEC_BCRYPT_PBKDF 0xd00d
#define MULTICODEC_ES256K 0xd0e7
#define MULTICODEC_BLS12_381_G1_SIG 0xd0ea
#define MULTICODEC_BLS12_381_G2_SIG 0xd0eb
#define MULTICODEC_EDDSA 0xd0ed
#define MULTICODEC_EIP_191 0xd191

/* JWK in JCS form: */
#define MULTICODEC_JWK_JCS_PUB 0xeb51

/* ============== 0xF100 Range (Filecoin) ============== */
#define MULTICODEC_FIL_COMMITMENT_UNSEALED 0xf101
#define MULTICODEC_FIL_COMMITMENT_SEALED 0xf102

/* ============== Shelter Protocol ============== */
#define MULTICODEC_SHELTER_CONTRACT_MANIFEST 0x511e00
#define MULTICODEC_SHELTER_CONTRACT_TEXT 0x511e01
#define MULTICODEC_SHELTER_CONTRACT_DATA 0x511e02
#define MULTICODEC_SHELTER_FILE_MANIFEST 0x511e03
#define MULTICODEC_SHELTER_FILE_CHUNK 0x511e04

/* ============== plaintextv2 ============== */
#define MULTICODEC_PLAINTEXTV2 0x706c61

/* ============== Holochain ============== */
#define MULTICODEC_HOLOCHAIN_ADR_V0 0x807124
#define MULTICODEC_HOLOCHAIN_ADR_V1 0x817124
#define MULTICODEC_HOLOCHAIN_KEY_V0 0x947124
#define MULTICODEC_HOLOCHAIN_KEY_V1 0x957124
#define MULTICODEC_HOLOCHAIN_SIG_V0 0xa27124
#define MULTICODEC_HOLOCHAIN_SIG_V1 0xa37124

/* ============== Additional Namespaces ============== */
#define MULTICODEC_SKYNET_NS 0xb19910
#define MULTICODEC_ARWEAVE_NS 0xb29910
#define MULTICODEC_SUBSPACE_NS 0xb39910
#define MULTICODEC_KUMANDRA_NS 0xb49910

/* ============== ES (ECDSA) Sig Algorithms ============== */
#define MULTICODEC_ES256 0xd01200
#define MULTICODEC_ES284 0xd01201
#define MULTICODEC_ES512 0xd01202
#define MULTICODEC_RS256 0xd01205

/* ============== MultiSig versions of above ============== */
#define MULTICODEC_ES256K_MSIG 0xd01300
#define MULTICODEC_BLS12_381_G1_MSIG 0xd01301
#define MULTICODEC_BLS12_381_G2_MSIG 0xd01302
#define MULTICODEC_EDDSA_MSIG 0xd01303
#define MULTICODEC_BLS12_381_G1_SHARE_MSIG 0xd01304
#define MULTICODEC_BLS12_381_G2_SHARE_MSIG 0xd01305
#define MULTICODEC_LAMPORT_MSIG 0xd01306
#define MULTICODEC_LAMPORT_SHARE_MSIG 0xd01307
#define MULTICODEC_ES256_MSIG 0xd01308
#define MULTICODEC_ES384_MSIG 0xd01309
#define MULTICODEC_ES521_MSIG 0xd0130a
#define MULTICODEC_RS256_MSIG 0xd0130b

/* ============== SCION ============== */
#define MULTICODEC_SCION 0xd02000

#endif /* MULTICODEC_CODES_H */