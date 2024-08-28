#pragma once
#include <string>
#include <Cryptopp/des.h>
#include <Cryptopp/aes.h>
#include <Cryptopp/rsa.h>
#include <Cryptopp/hex.h>
#include <Cryptopp/sha3.h>
#include <Cryptopp/files.h>
#pragma comment(lib, "cryptlib.lib")

using namespace CryptoPP;


class okcrypt final
{
public:

	// DES
	static void InitalizeDESKey();
	static const std::string GetDESKey();
	static void SetDESKey(std::string const& key);
	static void EncryptDES(std::string const& plainData, std::string& cipherData);
	static void DecryptDES(std::string const& cipherData, std::string& recoveredData);

	// 3DES
	static void Initalize3DESKey();
	static const std::string Get3DESKey();
	static void Set3DESKey(std::string const& key);
	static void Encrypt3DES(std::string const& plainData, std::string& cipherData);
	static void Decrypt3DES(std::string const& cipherData, std::string& recoveredData);

	// AES
	static void InitalizeAESKey();
	static const std::string GetAESKey();
	static void SetAESKey(std::string const& key);
	static void EncryptAES(std::string const& plainData, std::string& cipherData);
	static void DecryptAES(std::string const& cipherData, std::string& recoveredData);

	// RSA
	static void InitalizeRSAKeys(size_t bits = 1024);
	static const std::string GetRSAPublicKey();
	static void SetRSAPublicKey(std::string& key);
	static void EncryptRSA(std::string const& plainData, std::string& cipherData);
	static void DecryptRSA(std::string const& cipherData, std::string& recoveredData);

	// SHA2
	static const std::string EncryptSHA224(std::string const& msg);
	static const std::string EncryptSHA256(std::string const& msg);
	static const std::string EncryptSHA384(std::string const& msg);
	static const std::string EncryptSHA512(std::string const& msg);
	
	// SHA3
	static const std::string EncryptSHA3_224(std::string const& msg);
	static const std::string EncryptSHA3_256(std::string const& msg);
	static const std::string EncryptSHA3_384(std::string const& msg);
	static const std::string EncryptSHA3_512(std::string const& msg);

	// Base64
	static const std::string EncryptBase64(std::string const& plainData);

	// MD5
	static const std::string EncryptMD5(std::string const& msg);

	static const std::string ToHex(std::string const& digest) {
		std::stringstream ss;
		HexEncoder encoder(new FileSink(ss));
		(void)StringSource(digest, true, new Redirector(encoder));
		return ss.str();
	}

private:
	template<class SHAType>
	static const std::string EncryptSHA(std::string const& msg) {
		SHAType hash;
		std::string digest;
		hash.Update((const byte*)msg.data(), msg.size());
		digest.resize(hash.DigestSize());
		hash.Final((byte*)&digest[0]);
		
		return ToHex(digest);
	}

private:
	static SecByteBlock m_desKey;
	static byte m_desIV[DES_EDE2::BLOCKSIZE];

	static SecByteBlock m_3desKey;
	static byte m_3desIV[DES_EDE3::BLOCKSIZE];

	static SecByteBlock m_aesKey;
	static SecByteBlock m_aesIV;

	static RSA::PublicKey m_rsaPublicKey;
	static RSA::PrivateKey m_rsaPrivateKey;
};
