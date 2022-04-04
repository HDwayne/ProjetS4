package application;

import java.io.IOException;

/**
 * This class represents a file
 */
public class File {
    private int size;
    private byte[] data = new byte[OsDefines.MAX_FILE_SIZE];

    // This is the constructor of the class. It initializes the size and the data array.
    public File(){
        this.size = 0;
    }

    /**
     * It sets the size of the array.
     *
     * @param size The size of the array.
     */
    public void setSize(int size){
        this.size = size;
    }

    /**
     * This function copies the data from the array into the file
     *
     * @param data the data to be written to the file
     */
    public void setData(byte[] data) {
        if (this.size >= 0) System.arraycopy(data, 0, this.data, 0, this.size);
        if (this.size != OsDefines.MAX_FILE_SIZE){
            for (int i = data.length; i < OsDefines.MAX_FILE_SIZE; i++){
                this.data[i] = '\0';
            }
        }
    }

    /**
     * Erase the data from the disk
     *
     * @param disk the disk to write to
     * @param inode the inode of the file to be erased
     */
    public void eraseFromDisk(VirtualDisk disk, Inode inode) throws IOException {
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        Block block = new Block();
        block.clear();
        int nBlocks = Block.computeNBlock(this.size);
        for (int j = 0; j < nBlocks; j++, i++){
            block.writeBlock(disk, i);
        }
        if (this.size%4 != 0){
            block.writeBlock(disk, i);
        }
    }

    /**
     * Write the data of the file to the disk
     *
     * @param disk the disk to write to
     * @param inode the inode that this data block belongs to
     */
    public void write(VirtualDisk disk, Inode inode) throws IOException {
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        Block block = new Block();
        int nBlocks = this.size/OsDefines.BLOCK_SIZE;
        for (int j = 0; j < nBlocks; j++, i++){
            block.clear();
            for (int k = 0; k < OsDefines.BLOCK_SIZE; k++){
                block.setData(k, this.data[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
        if (this.size%4 != 0){
            block.clear();
            for (int k = 0; k < this.size%4; k++){
                block.setData(k, this.data[nBlocks*OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
    }

    /**
     * Read the data from the disk and store it in the data field
     *
     * @param disk The VirtualDisk object that contains the data.
     * @param inode the inode of the file
     */
    public void read(VirtualDisk disk, Inode inode) throws IOException {
        this.setSize(inode.getSize());
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        byte[] buffer = new byte[this.size];
        Block.readBlocks(disk, buffer, i);
        this.setData(buffer);
    }

    /**
     * Given a string, replace all the null characters with empty strings
     *
     * @return The data in the packet, with null characters removed.
     */
    public String toString(){
        String data = new String(this.data);
        data = data.replace("\0", "");
        return data;
    }
}
