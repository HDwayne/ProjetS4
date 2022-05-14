package application;

import java.io.IOException;

public class SuperBlock {
    private static int numberOfFiles = 0;
    private static int numberOfUsers = 0;
    private static int nbBlocsUsed = 0;
    private static int firstFreeByte = 0;

    /**
     * Set the first free byte in the superblock to the given value
     *
     * @param firstFreeByte the first byte of the file system that is not allocated to any file.
     */
    public static void setFirstFreeByte(int firstFreeByte) {
        SuperBlock.firstFreeByte = firstFreeByte;
    }

    /**
     * If the first inode is free, set the first free byte to the first inode's first byte. Otherwise, set the first free
     * byte to the first byte of the first inode that is not free
     *
     * @param disk the disk to update
     */
    public static void updateFirstByte(VirtualDisk disk) {
        int ffb = OsDefines.USERS_START + OsDefines.USER_SIZE * OsDefines.USER_SIZE * OsDefines.BLOCK_SIZE;
        if (disk.getInode(0).isFree()){
            setFirstFreeByte(ffb);
        }
        else {
            int i = 0;
            while(!disk.getInode(i).isFree()){
                ffb = disk.getInode(i).getFirstByte() + disk.getInode(i).getnBlock() * OsDefines.BLOCK_SIZE;
                i++;
            }
            setFirstFreeByte(ffb);
        }
    }

    /**
     * Write the number of files, number of users, number of blocks used and the first free byte to the disk
     *
     * @param disk the disk to write to
     */
    public static void write(VirtualDisk disk) throws IOException {
        Block block = new Block();

        block.fromInt(numberOfFiles);
        block.writeBlock(disk,0);

        block.fromInt(numberOfUsers);
        block.writeBlock(disk,1);

        block.fromInt(nbBlocsUsed);
        block.writeBlock(disk,2);

        block.fromInt(firstFreeByte);
        block.writeBlock(disk,3);
    }

    /**
     * Reads the first block of the disk and stores the number of files, number of users, number of blocks used and the
     * first free byte
     *
     * @param disk the disk to read from
     */
    public static void read(VirtualDisk disk) throws IOException {
        Block block = new Block();
        block.readBlock(disk,0);
        numberOfFiles = block.toInt();

        block.readBlock(disk,1);
        numberOfUsers = block.toInt();

        block.readBlock(disk,2);
        nbBlocsUsed = block.toInt();

        block.readBlock(disk,3);
        firstFreeByte = block.toInt();
    }

    /**
     * Print the superblock
     */
    public static void print() {
        System.out.print("SuperBlock { ");
        System.out.print("Number of files: " + numberOfFiles + ", ");
        System.out.print("Number of users: " + numberOfUsers + ", ");
        System.out.print("Number of blocks used: " + nbBlocsUsed +", ");
        System.out.println("First free byte: " + firstFreeByte + " }");
    }

    public static int getNumberOfFiles(){
        return numberOfFiles;
    }
    public static int getNumberOfUsers(){
        return numberOfUsers;
    }
    public static int getNbBlocsUsed(){
        return nbBlocsUsed;
    }
    public static int getFirstFreeByte(){
        return firstFreeByte;
    }
}


