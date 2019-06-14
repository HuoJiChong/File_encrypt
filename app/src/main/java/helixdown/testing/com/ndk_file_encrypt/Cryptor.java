package helixdown.testing.com.ndk_file_encrypt;

public class Cryptor {


    /**
     * 待加密的文件路径名
     * @param normal_path
     * @param crypt_path
     */
    public native static void Encrypt(String normal_path,String crypt_path);

    /**
     * 待解密的文件路径名
     * @param crypt_path
     * @param decrypt_path
     */
    public native static void Decrypt(String crypt_path,String decrypt_path);

}
