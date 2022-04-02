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
        System.out.println();
        str += "Inode Table :\n";
        StringBuilder strBuilder = new StringBuilder(str);
        for (int i = 0; i < this.tabInode.length; i++) {
            if(!this.tabInode[i].isFree()){
                strBuilder.append("inode ").append(i).append(" :\n").append(this.tabInode[i].toString()).append("\n");
            }
        }
        str = strBuilder.toString();

        str += "User Table :\n";
        StringBuilder strBuilder1 = new StringBuilder(str);
        for (int i = 0; i < this.tabUser.length; i++) {
            if(!this.tabUser[i].isFree()){
                strBuilder1.append("user ").append(i).append(" :\n").append(this.tabUser[i].toString()).append("\n");
            }
        }
        str = strBuilder1.toString();
        return str;
    }

    public Inode getInode(int inodeId){
        return this.tabInode[inodeId];
    }

    public void defragmentation() throws IOException {
        int max = 0;
        int minmax = 0;
        if (!this.tabInode[0].isFree() && this.tabInode[0].getFirstByte() != OsDefines.USERS_START + (OsDefines.USER_SIZE * OsDefines.NB_USERS * OsDefines.BLOCK_SIZE)) {
            System.out.println("[Défragmentaion] Inode 0");
            max = this.tabInode[0].getFirstByte() + this.tabInode[0].getnBlock() * OsDefines.BLOCK_SIZE;
            File file = new File();
            file.read(this, this.tabInode[0]);
            file.eraseFromDisk(this, this.tabInode[0]);
            this.tabInode[0].setFirstByte(OsDefines.USERS_START + (OsDefines.USER_SIZE * OsDefines.NB_USERS * OsDefines.BLOCK_SIZE));
            file.write(this, this.tabInode[0]);
            minmax = this.tabInode[0].getFirstByte() + this.tabInode[0].getnBlock() * OsDefines.BLOCK_SIZE;
        }

        for (int i = 1; i < OsDefines.INODE_TABLE_SIZE; i++) {
            if (!this.tabInode[i].isFree() && this.tabInode[i].getFirstByte() != this.tabInode[i-1].getFirstByte() + this.tabInode[i-1].getnBlock()*4){
                max = this.tabInode[i].getFirstByte() + this.tabInode[i].getnBlock() * OsDefines.BLOCK_SIZE;
                System.out.println("[Défragmentaion] Inode " + i);
                File file = new File();
                file.read(this, this.tabInode[i]);
                file.eraseFromDisk(this, this.tabInode[i]);
                this.tabInode[i].setFirstByte(this.tabInode[i-1].getFirstByte() + this.tabInode[i-1].getnBlock()*4);
                file.write(this, this.tabInode[i]);
                minmax = this.tabInode[i].getFirstByte() + this.tabInode[i].getnBlock() * OsDefines.BLOCK_SIZE;
            }
        }
        SuperBlock.updateFirstByte(this);
        System.out.println("[Défragmentation] Espace sauvé : " + (max - minmax));
    }
}
