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
        for (int j = 0; j < Block.computeNBlock(OsDefines.FILENAME_MAX_SIZE); j++, i++){
            for (int k = 3; k >= 0; k--) {
                block.setData(k, this.login[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }

        for (int j = 0; j < Block.computeNBlock(OsDefines.PASSWORD_MAX_SIZE)-1; j++, i++){
            for (int k = 3; k >= 0; k--) {
                block.setData(k, this.passwd[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
        block.setData(3, this.passwd[OsDefines.PASSWORD_MAX_SIZE-1]);
        for (int k = 2; k >= 0; k--) {
            block.setData(k, (byte)'\0');
        }
        block.writeBlock(disk, i);


    }

    public void read(VirtualDisk disk, int userId) throws IOException {
        Block block = new Block();

        byte[] username = new byte[OsDefines.FILENAME_MAX_SIZE];
        int i = OsDefines.USERS_START/OsDefines.BLOCK_SIZE + userId * OsDefines.USER_SIZE;
        for(int j = 0; j < Block.computeNBlock(OsDefines.FILENAME_MAX_SIZE); j++, i++){
            block.readBlock(disk, i);
            for (int k = 3; k >= 0; k--) {
                username[j*OsDefines.BLOCK_SIZE + k] = block.getData(k);
            }
        }

        byte[] password = new byte[OsDefines.PASSWORD_MAX_SIZE];
        for(int j = 0; j < Block.computeNBlock(OsDefines.PASSWORD_MAX_SIZE)-1; j++, i++){
            block.readBlock(disk, i);
            for (int k = 3; k >= 0; k--) {
                password[j*OsDefines.BLOCK_SIZE + k] = block.getData(k);
            }
        }
        block.readBlock(disk, i);
        password[OsDefines.PASSWORD_MAX_SIZE-1] = block.getData(3);

        this.init(username, password);
    }
}
