package application;

import java.io.IOException;

public class User {
    private byte[] login = new byte[OsDefines.FILENAME_MAX_SIZE];
    private byte[] passwd = new byte[OsDefines.PASSWORD_MAX_SIZE];

    // This is the default constructor of the class. It is used to initialize the object with default values.
    public User() {
        this.setLogin("\0".getBytes());
        this.setPasswd("\0".getBytes());
    }

    /**
     * It sets the login field to the value of the login parameter.
     *
     * @param login The login name of the user.
     */
    public void setLogin(byte[] login) {
        this.login = login;
    }

    /**
     * It sets the password for the user.
     *
     * @param passwd The password to use for the user.
     */
    public void setPasswd(byte[] passwd) {
        this.passwd = passwd;
    }


    /**
     * This function initializes the class with the login and password
     *
     * @param login The login name of the user.
     * @param passwd The password to use for the user.
     */
    public void init(byte[] login, byte[] passwd) {
        this.login = login;
        this.passwd = passwd;
    }

    /**
     * It prints the login and password of the user.
     *
     * @return The string representation of the user.
     */
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

    /**
     * Return if the user exists
     *
     * @return A boolean value.
     */
    public boolean isFree() {
        return login[0] == 0;
    }

    /**
     * Write the user's login and password to the disk
     *
     * @param disk the VirtualDisk object that represents the disk.
     * @param userId the user's id
     */
    public void write(VirtualDisk disk, int userId) throws IOException {
        Block block = new Block();
        int i = OsDefines.USERS_START/OsDefines.BLOCK_SIZE + userId * OsDefines.USER_SIZE;
        i = Block.writeBlocks(disk, this.login, i);
        i = Block.writeBlocks(disk, this.passwd, i);
        block.writeBlock(disk, i);
    }

    /**
     * Reads the username and password from the disk
     *
     * @param disk The VirtualDisk object that contains the file.
     * @param userId The user ID of the user to read.
     */
    public void read(VirtualDisk disk, int userId) throws IOException {
        byte[] username = new byte[OsDefines.FILENAME_MAX_SIZE];
        int i = OsDefines.USERS_START/OsDefines.BLOCK_SIZE + userId * OsDefines.USER_SIZE;
        i = Block.readBlocks(disk, username, i);

        byte[] password = new byte[OsDefines.PASSWORD_MAX_SIZE];
        Block.readBlocks(disk, password, i);

        this.init(username, password);
    }

    public String analysis(VirtualDisk disk, int userId) throws IOException {
        String result = "Analyse de l'utilisateur n°" + userId + "\n";
        int size = 0;
        for (int i = 0; i < OsDefines.PASSWORD_MAX_SIZE && this.passwd[i] != 0; i++) {
            size++;
        }

        if (size != OsDefines.PASSWORD_MAX_SIZE-1) {
            result += "Le mot de passe contient " + size + " caractères il est donc probablement corrompu\n";
        }else{
            result += "Le mot de passe contient " + size + " caractères il est donc correct\n";
        }

        return result;
    }
}
