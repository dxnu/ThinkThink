#include "okcrypt.h"
#include <CryptoPP/osrng.h>
#include <Cryptopp/modes.h>
#include <Cryptopp/base64.h>
#include <Cryptopp/md5.h>


SecByteBlock        okcrypt::m_desKey(0x00, DES_EDE2::DEFAULT_KEYLENGTH);
byte                okcrypt::m_desIV[DES_EDE2::BLOCKSIZE];

SecByteBlock        okcrypt::m_3desKey(0x00, DES_EDE3::DEFAULT_KEYLENGTH);
byte                okcrypt::m_3desIV[DES_EDE3::BLOCKSIZE];

SecByteBlock        okcrypt::m_aesKey(0x00, AES::DEFAULT_KEYLENGTH);
SecByteBlock        okcrypt::m_aesIV(AES::BLOCKSIZE);

RSA::PublicKey      okcrypt::m_rsaPublicKey;
RSA::PrivateKey     okcrypt::m_rsaPrivateKey;


void okcrypt::InitalizeDESKey()
{
	AutoSeededRandomPool rng;

	// ������Կ��IV
	rng.GenerateBlock(m_desKey, m_desKey.size());
	rng.GenerateBlock(m_desIV, sizeof(m_desIV));
}

const std::string okcrypt::GetDESKey()
{
	std::string key((char*)m_desKey.data(), m_desKey.size());
	return key;
}

void okcrypt::SetDESKey(std::string const& key)
{
	SecByteBlock s((const byte*)key.data(), key.size());
	m_desKey = s;
}

void okcrypt::EncryptDES(std::string const& plainData, std::string &cipherData)
{
	try {
		CBC_Mode<DES_EDE2>::Encryption e;
		e.SetKeyWithIV(m_desKey, m_desKey.size(), m_desIV);

		StringSource ss(plainData, true,
			new StreamTransformationFilter(e, new StringSink(cipherData))
		);
	}
	catch (const CryptoPP::Exception &e)
	{
		throw e;
	}
}

void okcrypt::DecryptDES(std::string const& cipherData, std::string &recoveredData)
{
	try {
		CBC_Mode<DES_EDE2>::Decryption d;
		d.SetKeyWithIV(m_desKey, m_desKey.size(), m_desIV);

		StringSource ss(cipherData, true,
			new StreamTransformationFilter(d, new StringSink(recoveredData))
		);
	}
	catch (const CryptoPP::Exception &e)
	{
		throw e;
	}
}

void okcrypt::Initalize3DESKey()
{
	AutoSeededRandomPool rng;

	rng.GenerateBlock(m_3desKey, m_3desKey.size());
	rng.GenerateBlock(m_3desIV, sizeof(m_3desIV));
}

const std::string okcrypt::Get3DESKey()
{
	std::string key((char *)m_3desKey.data(), m_3desKey.size());
	return key;
}

void okcrypt::Set3DESKey(std::string const& key)
{
	SecByteBlock s((const byte*)key.data(), key.size());
	m_3desKey = s;
}

void okcrypt::Encrypt3DES(std::string const& plainData, std::string &cipherData)
{
	try {
		CBC_Mode<DES_EDE3>::Encryption e;
		e.SetKeyWithIV(m_3desKey, m_3desKey.size(), m_3desIV);

		StringSource ss(plainData, true,
			new StreamTransformationFilter(e, new StringSink(cipherData))
		);
	}
	catch (const CryptoPP::Exception& e)
	{
		throw e;
	}
}

void okcrypt::Decrypt3DES(std::string const& cipherData, std::string &recoveredData)
{
	try {
		CBC_Mode<DES_EDE3>::Decryption d;
		d.SetKeyWithIV(m_3desKey, m_3desKey.size(), m_3desIV);

		StringSource ss(cipherData, true,
			new StreamTransformationFilter(d, new StringSink(recoveredData))
		);
	}
	catch (const CryptoPP::Exception& e)
	{
		throw e;
	}
}

void okcrypt::InitalizeAESKey()
{
	AutoSeededRandomPool rng;

	// ���������Կ
	rng.GenerateBlock(m_aesKey, m_aesKey.size());

	// �������IV
	rng.GenerateBlock(m_aesIV, m_aesIV.size());
}

const std::string okcrypt::GetAESKey()
{
	std::string key((char*)m_aesKey.data(), m_aesKey.size());
	return key;
}

void okcrypt::SetAESKey(std::string const& key)
{
	SecByteBlock s((const byte*)key.data(), key.size());
	m_aesKey = s;
}

void okcrypt::EncryptAES(std::string const& plainData, std::string &cipherData)
{
	// ����
	CFB_Mode<AES>::Encryption e(m_aesKey, m_aesKey.size(), m_aesIV);
	cipherData.resize(plainData.size());
	e.ProcessData((byte*)&cipherData[0], (byte*)plainData.data(), plainData.size());
}

void okcrypt::DecryptAES(std::string const& cipherData, std::string &recoveredData)
{
	// ����
	CFB_Mode<AES>::Decryption d(m_aesKey, m_aesKey.size(), m_aesIV);
	recoveredData.resize(cipherData.size());
	d.ProcessData((byte*)&recoveredData[0], (byte*)cipherData.data(), cipherData.size());
}

void okcrypt::InitalizeRSAKeys(size_t bits)
{
	// α�����������
	AutoSeededRandomPool rng;

	// ���ɲ���
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, bits);

	// ������Կ��
	m_rsaPrivateKey = params;
	m_rsaPublicKey = params;
}

const std::string okcrypt::GetRSAPublicKey()
{
	std::string pubKey;
	StringSink ss(pubKey);
	m_rsaPublicKey.Save(ss);

	return pubKey;
}

void okcrypt::SetRSAPublicKey(std::string& key)
{
	StringSink ss(key);
	m_rsaPublicKey.Load(ss);
}

void okcrypt::EncryptRSA(std::string const& plainData, std::string &cipherData)
{
	// α�����������
	AutoSeededRandomPool rng;

	// ������
	RSAES_OAEP_SHA_Encryptor e(m_rsaPublicKey);

	StringSource ss(plainData, true,
		new PK_EncryptorFilter(rng, e, new StringSink(cipherData))
	);
}

void okcrypt::DecryptRSA(std::string const& cipherData, std::string &recoveredData)
{
	// α�����������
	AutoSeededRandomPool rng;

	// ������
	RSAES_OAEP_SHA_Decryptor d(m_rsaPrivateKey);

	StringSource ss(cipherData, true,
		new PK_DecryptorFilter(rng, d, new StringSink(recoveredData))
	);
}

const std::string okcrypt::EncryptSHA224(std::string const& msg)
{
	return EncryptSHA<SHA224>(msg);
}

const std::string okcrypt::EncryptSHA256(std::string const& msg)
{
	return EncryptSHA<SHA256>(msg);
}

const std::string okcrypt::EncryptSHA384(std::string const& msg)
{
	return EncryptSHA<SHA384>(msg);
}

const std::string okcrypt::EncryptSHA512(std::string const& msg)
{
	return EncryptSHA<SHA512>(msg);
}

const std::string okcrypt::EncryptSHA3_224(std::string const& msg)
{
	return EncryptSHA<SHA3_224>(msg);
}

const std::string okcrypt::EncryptSHA3_256(std::string const& msg)
{
	return EncryptSHA<SHA3_256>(msg);
}

const std::string okcrypt::EncryptSHA3_384(std::string const& msg)
{
	return EncryptSHA<SHA3_384>(msg);
}

const std::string okcrypt::EncryptSHA3_512(std::string const& msg)
{
	return EncryptSHA<SHA3_512>(msg);
}

const std::string okcrypt::EncryptBase64(std::string const& plainData)
{
	std::string encoded;

	StringSource ss((const byte*)plainData.data(), plainData.size(), 
		true, new Base64Encoder(new StringSink(encoded))
	);

	return encoded;
}

const std::string okcrypt::EncryptMD5(std::string const& msg)
{	
	std::string digest;
	Weak1::MD5 hash;
	hash.Update((const byte*)msg.data(), msg.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);

	return ToHex(digest);
}