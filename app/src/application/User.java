package application;

import java.io.IOException;

public class User {
    private byte[] login = new byte[OsDefines.FILENAME_MAX_SIZE];
    private byte[] passwd = new byte[OsDefines.PASSWORD_MAX_SIZE];

    public User() {
        this.setLogin("\0".getBytes());
        this.setPasswd("\0".getBytes());
    }

    public void setLogin(byte[] login) {
        this.login = login;
    }

    public void setPasswd(byte[] passwd) {
        this.passwd = passwd;
    }


    public void init(byte[] login, byte[] passwd) {
        this.login = login;
        this.passwd = passwd;
    }

    public String toString(){
        StringBuilder str = new StringBuilder("User : ");
        for(int i = 0; i < OsDefines.FILENAME_MAX_SIZE; i++){
            if(login[i] != 0)
                str.append((char) login[i]);
        }
        str.append("\nPassword : ");
        for(int i = 0; i < OsDefines.PASSWORD_MAX_SIZE; i++){
            if(passwd[i] != 0)
                str.append((char) passwd[i]);
        }

        return str.toString();
    }

    public boolean isFree() {
        return login[0] == 0;
    }

    public void write(VirtualDisk disk, int userId) throws IOException {
        Block block = new Block();
        int i = OsDefines.USERS_START/OsDefines.BLOCK_SIZE + userId * OsDefines.USER_SIZE;
        i = Block.writeBlocks(disk, this.login, i);
        i = Block.writeBlocks(disk, this.passwd, i);
        block.writeBlock(disk, i);


    }

    public void read(VirtualDisk disk, int userId) throws IOException {
        byte[] username = new byte[OsDefines.FILENAME_MAX_SIZE];
        int i = OsDefines.USERS_START/OsDefines.BLOCK_SIZE + userId * OsDefines.USER_SIZE;
        i = Block.readBlocks(disk, username, i);

        byte[] password = new byte[OsDefines.PASSWORD_MAX_SIZE];
        Block.readBlocks(disk, password, i);

        this.init(username, password);
    }
}
