package application;

public class OsDefines {
    public final static int BLOCK_SIZE = 4;
    public final static int FILENAME_MAX_SIZE = 32;
    public final static int MAX_FILE_SIZE = 2*1024;
    public final static int INODE_TABLE_SIZE = 10;
    public final static int MAX_MSG = 1024;
    public final static int TIMESTAMP_SIZE = 24+2;
    public final static int NB_USERS = 5;
    public final static int SUPER_BLOCK_SIZE = 4;
    public final static int INODES_START = SUPER_BLOCK_SIZE * BLOCK_SIZE;
    public final static int INODE_SIZE = ((FILENAME_MAX_SIZE/BLOCK_SIZE+6+((TIMESTAMP_SIZE)*2)/BLOCK_SIZE)+1);
    public final static int USERS_START = ((INODES_START)+(INODE_SIZE)*(INODE_TABLE_SIZE)*(BLOCK_SIZE));
    public final static int PASSWORD_MAX_SIZE = 65;
    public final static int USER_SIZE = ((FILENAME_MAX_SIZE/BLOCK_SIZE)+(PASSWORD_MAX_SIZE/BLOCK_SIZE)+2);

    public final static String PATH = "./repdisk/";


}
