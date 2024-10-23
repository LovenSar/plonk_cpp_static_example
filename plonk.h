// plonk.h

#ifndef PLONK_H
#define PLONK_H

#include <vector>
#include <string>
#include <map>
#include <tuple>

// Include libff headers
#include <libff/algebra/field_utils/field_utils.hpp>
#include <libff/algebra/curves/alt_bn128/alt_bn128_pp.hpp>

// Include JSON library
#include <nlohmann/json.hpp>

// Type aliases
using FieldT = libff::Fr<libff::alt_bn128_pp>;
using G1 = libff::G1<libff::alt_bn128_pp>;
using G2 = libff::G2<libff::alt_bn128_pp>;

// Initialize curve parameters
void init_curve();

// Setup class
class Setup {
public:
    std::vector<G1> G1_points;
    G2 X2;

    Setup(const std::string& filename);
};

// VerificationKey structure
struct VerificationKey {
    G1 Qm, Ql, Qr, Qo, Qc, S1, S2, S3;
    G2 X_2;
    FieldT w;
};

// Function declarations
VerificationKey make_verification_key(const Setup& setup, size_t group_order, const std::vector<std::string>& code);

std::tuple<G1, G1, G1, G1, G1, G1, G1, G1, G1,
           FieldT, FieldT, FieldT, FieldT, FieldT, FieldT>
prove_from_witness(const Setup& setup, size_t group_order, const std::vector<std::string>& code,
                   const std::map<std::string, uint64_t>& var_assignments);

bool verify_proof(const Setup& setup, size_t group_order, const VerificationKey& vk,
                  const std::tuple<G1, G1, G1, G1, G1, G1, G1, G1, G1,
                                   FieldT, FieldT, FieldT, FieldT, FieldT, FieldT>& proof,
                  const std::vector<uint64_t>& public_inputs, bool optimized = true);

// Serialization functions
nlohmann::json serialize_proof(const std::tuple<G1, G1, G1, G1, G1, G1, G1, G1, G1,
                                                FieldT, FieldT, FieldT, FieldT, FieldT, FieldT>& proof);

std::tuple<G1, G1, G1, G1, G1, G1, G1, G1, G1,
           FieldT, FieldT, FieldT, FieldT, FieldT, FieldT>
deserialize_proof(const nlohmann::json& j);

#endif // PLONK_H
