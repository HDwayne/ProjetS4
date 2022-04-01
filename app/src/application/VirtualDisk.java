package application;

import java.io.*;

public class VirtualDisk {
    private Inode[] tabInode = new Inode[OsDefines.INODE_TABLE_SIZE];
    private User[] tabUser = new User[OsDefines.NB_USERS];
    private RandomAccessFile storage;

    public VirtualDisk() throws IOException {
        this.storage = new RandomAccessFile(OsDefines.PATH + "d0", "rw");

        for (int i = 0; i < this.tabInode.length; i++) {
            this.tabInode[i] = new Inode();
        }
        for (int i = 0; i < this.tabUser.length; i++) {
            this.tabUser[i] = new User();
        }

        if(this.storage.length() == 0){
            System.out.println("Disque vide.");
        }
        else{
            this.readSuperBlock();
            this.readInodeTable();
            this.readUserTable();
        }
    }

    public void saveDisk(){
        try {
            this.writeSuperBlock();
            this.writeInodeTable();
            this.writeUserTable();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void readInodeTable() throws IOException {
        for (int i = 0; i < this.tabInode.length; i++) {
            this.tabInode[i].read(this, i);
        }
    }

    public void readUserTable() throws IOException {
        for (int i = 0; i < this.tabUser.length; i++) {
            this.tabUser[i].read(this, i);
        }
    }

    public void writeInodeTable() throws IOException {
        for (int i = 0; i < this.tabInode.length; i++) {
            this.tabInode[i].write(this, i);
        }
    }

    public void writeSuperBlock() throws IOException {
       SuperBlock.write(this);
    }

    public void writeUserTable() throws IOException {
        for (int i = 0; i < this.tabUser.length; i++) {
            this.tabUser[i].write(this, i);
        }
    }

    public RandomAccessFile getStorage(){
        return this.storage;
    }

    public void readSuperBlock() throws IOException{
        SuperBlock.read(this);
    }

    public String toString(){
        String str="";

        SuperBlock.print();
        System.out.println("");
        str += "Inode Table :\n";
        for (int i = 0; i < this.tabInode.length; i++) {
            if(!this.tabInode[i].isFree()){
                str += "inode " + i + " :\n" + this.tabInode[i].toString() + "\n";
            }
        }

        str += "User Table :\n";
        for (int i = 0; i < this.tabUser.length; i++) {
            if(!this.tabUser[i].isFree()){
                str += "user " + i + " :\n" + this.tabUser[i].toString() + "\n";
            }
        }
        return str;
    }

    public Inode getInode(int inodeId){
        return this.tabInode[inodeId];
    }
}
