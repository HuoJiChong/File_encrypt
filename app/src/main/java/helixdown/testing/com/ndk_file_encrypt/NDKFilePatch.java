package helixdown.testing.com.ndk_file_encrypt;

public class NDKFilePatch {
    /**
     *
     * @param path
     * @param path_pattern
     * @param count
     */
    public native static void Diff(String path,String path_pattern,int count);

    /**
     *
     * @param path_pattern
     * @param count
     * @param path_patch
     */
    public native static void Patch(String path_pattern,int count,String path_patch);
}
