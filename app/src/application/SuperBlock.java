package application;

import java.io.IOException;

public class SuperBlock {
    private static int numberOfFiles = 0; // dans super bloc
    private static int numberOfUsers = 0; // idem
    private static int nbBlocsUsed = 0; //
    private static int firstFreeByte = 0;

    public static void setFirstFreeByte(int firstFreeByte) {
        SuperBlock.firstFreeByte = firstFreeByte;
    }

    public static void updateFirstByte(VirtualDisk disk) {
        int ffb = OsDefines.USERS_START + OsDefines.USER_SIZE * OsDefines.USER_SIZE * OsDefines.BLOCK_SIZE;
        if (disk.getInode(0).isFree()){
            setFirstFreeByte(ffb);
        }
        else {
            int i = 0;
            while(!disk.getInode(i).isFree()){
                ffb = disk.getInode(i).getFirstByte() + disk.getInode(i).getnBlock() * OsDefines.BLOCK_SIZE;
            }
            setFirstFreeByte(ffb);
        }
    }

    public static void write(VirtualDisk disk) throws IOException {
        Block block = new Block();

        for (int i = 3; i >= 0; i--){
            block.setData(i, (byte) ((numberOfFiles >> (i * 8)) & 0xFF)); // on écrit le nombre de fichiers
        }
        block.writeBlock(disk,0);

        for (int i = 3; i >= 0; i--){
            block.setData(i, (byte) ((numberOfUsers >> (i * 8)) & 0xFF)); // on écrit le nombre d'utilisateurs
        }
        block.writeBlock(disk,1);

        for (int i = 3; i >= 0; i--){
            block.setData(i, (byte) ((nbBlocsUsed >> (i * 8)) & 0xFF)); // on écrit le nombre de blocs utilisés
        }
        block.writeBlock(disk,2);

        for (int i = 3; i >= 0; i--){
            block.setData(i, (byte) ((firstFreeByte >> (i * 8)) & 0xFF)); // on écrit le premier bloc libre
        }
        block.writeBlock(disk,3);
    }

    public static void read(VirtualDisk disk) throws IOException {
        Block block = new Block();

        block.readBlock(disk,0);
        for (int j = 3; j >= 0; j--){
            numberOfFiles = (numberOfFiles << 8) + (block.getData(j) & 0xFF);
        }

        block.readBlock(disk,1);
        for (int j = 3; j >= 0; j--){
            numberOfUsers = (numberOfUsers << 8) + (block.getData(j) & 0xFF);
        }

        block.readBlock(disk,2);
        for (int j = 3; j >= 0; j--){
            nbBlocsUsed = (nbBlocsUsed << 8) + (block.getData(j) & 0xFF);
        }
        block.readBlock(disk,3);
        for (int j = 3; j >= 0; j--){
            firstFreeByte = (firstFreeByte << 8) + (block.getData(j) & 0xFF);
        }
    }

    public static void print() {
        System.out.print("SuperBlock { ");
        System.out.print("Number of files: " + numberOfFiles + ", ");
        System.out.print("Number of users: " + numberOfUsers + ", ");
        System.out.print("Number of blocks used: " + nbBlocsUsed +", ");
        System.out.println("First free byte: " + firstFreeByte + " }");
    }
}


