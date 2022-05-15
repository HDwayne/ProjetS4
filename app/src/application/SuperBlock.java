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
     * This function returns the first free byte.
     *
     * @return The first free byte in the memory.
     */
    public static int getFirstFreeByte(){
        return firstFreeByte;
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

    /**
     * It checks if the number of files, users, blocks used and the first free byte are correct
     *
     * @param disk the virtual disk
     * @return The result of the analysis of the superblock.
     */
    public static String analysis(VirtualDisk disk) {
        if (numberOfFiles == 0){
            return "Pas de données à analyser\n";
        }

        String result = "Cohérence du superblock\n";

        // Check if the number of files is correct
        int nbFiles = 0;
        for (int i = 0; i < OsDefines.INODE_TABLE_SIZE && !disk.getInode(i).isFree(); i++)
            nbFiles++;

        if (nbFiles != numberOfFiles) {
            result += "Nombre de fichiers incorrect " + nbFiles + " au lieu de " + numberOfFiles + "\n";
        } else {
            result += "Nombre de fichiers correct " + nbFiles + " fichiers trouvés\n";
        }

        // Check if the number of users is correct
        int nbUsers = 0;
        for (int i = 0; i < OsDefines.NB_USERS && !disk.getUser(i).isFree(); i++)
            nbUsers++;

        if (nbUsers != numberOfUsers) {
            result += "Nombre d'utilisateurs incorrect " + nbUsers + " au lieu de " + numberOfUsers + "\n";
        } else {
            result += "Nombre d'utilisateurs correct " + nbUsers + " utilisateurs trouvés\n";
        }

        // Check if the number of blocks used is correct
        int nbBlocsUsedFound = 0;
        for (int i = 0; i < OsDefines.INODE_TABLE_SIZE && !disk.getInode(i).isFree(); i++) {
            nbBlocsUsedFound += disk.getInode(i).getnBlock();
        }

        if (nbBlocsUsedFound != nbBlocsUsed) {
            result += "Nombre de blocs utilisés incorrect " + nbBlocsUsedFound + " au lieu de " + nbBlocsUsed + "\n";
        } else{
            result += "Nombre de blocs utilisés correct " + nbBlocsUsedFound + " blocs trouvés\n";
        }

        // Check if the first free byte is correct
        int firstfreebyteFound = disk.getInode(nbFiles-1).getFirstByte() + disk.getInode(nbFiles-1).getnBlock() * OsDefines.BLOCK_SIZE;
        if (firstfreebyteFound != firstFreeByte) {
            result += "Premier bloc libre incorrect " + firstfreebyteFound + " au lieu de " + firstFreeByte + "\n";
        }
        else {
            result += "Premier bloc libre correct " + firstfreebyteFound + " bloc trouvé\n";
        }

        return result;
    }
}


