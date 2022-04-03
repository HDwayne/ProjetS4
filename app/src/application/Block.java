package application;

import java.io.IOException;

public class Block {
    // This is creating a byte array of size BLOCK_SIZE.
    private final byte[] data = new byte[OsDefines.BLOCK_SIZE];


    // This is creating a byte array of size BLOCK_SIZE.
    public Block(){
        this.clear();
    }

    /**
     * Given an index, return the data at that index
     *
     * @param index The index of the data to retrieve.
     * @return The data at the given index.
     */
    public byte getData(int index) {
        return data[index];
    }

    /**
     * Set the value of the byte at the given index in the data array
     *
     * @param index The index of the byte to set.
     * @param value The value to be set.
     */
    public void setData(int index, byte value) {
        data[index] = value;
    }

    /**
     * Compute the number of blocks needed to store a file of a given size
     *
     * @param size the size of the file in bytes
     * @return The number of blocks that the file will occupy.
     */
    public static int computeNBlock(int size) {
        return size / OsDefines.BLOCK_SIZE;
    }

    /**
     * Write the block to the disk
     *
     * @param disk The VirtualDisk object that represents the disk.
     * @param position The position of the block in the file.
     */
    public void writeBlock(VirtualDisk disk, int position) throws IOException {
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        disk.getStorage().write(this.data);
    }

    /**
     * Reads a block from the disk
     *
     * @param disk The VirtualDisk object that contains the data to be read.
     * @param position The position of the block to read.
     */
    public void readBlock(VirtualDisk disk, int position) throws IOException {
        this.clear();
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        for (int i = 0; i < OsDefines.BLOCK_SIZE; i++) {
            data[i] = disk.getStorage().readByte();
        }
    }

    /**
     * Convert the byte array to an integer
     *
     * @return The integer value of the byte array.
     */
    public int toInt(){
        int n = 0;
        for(int i=OsDefines.BLOCK_SIZE-1;i>=0;i--){
            n = (n << 8) + (data[i] & 0xff);
        }
        return n;
    }

    /**
     * This function takes an integer and converts it to a byte array
     *
     * @param n The integer to be converted to a byte array.
     */
    public void fromInt(int n){
        this.clear();
        for(int i=OsDefines.BLOCK_SIZE - 1;i >= 0;i--){
            this.setData(i, (byte) ((n >> (i * 8)) & 0xFF));
        }
    }

    /**
     * Reads the blocks of data from the disk and places them in the buffer
     *
     * @param disk the disk to read from
     * @param buffer the buffer to read into
     * @param position the position of the first block to read
     * @return The position of the next block to be read.
     */
    public static int readBlocks(VirtualDisk disk, byte[] buffer, int position) throws IOException {
        Block block = new Block();
        int k;
        int nblock = Block.computeNBlock(buffer.length);
        for (k = 0; k < nblock; k++, position++) {
            block.clear();
            block.readBlock(disk, position);
            for (int j = 3; j >= 0; j--) {
                buffer[k*OsDefines.BLOCK_SIZE + j] = block.getData(j);
            }
        }
        if(buffer.length % OsDefines.BLOCK_SIZE != 0) {
            block.clear();
            block.readBlock(disk, position);
            int i = 0;
            for (int j = (OsDefines.BLOCK_SIZE - (buffer.length%OsDefines.BLOCK_SIZE));j < OsDefines.BLOCK_SIZE; j++, i++) {
                buffer[k*OsDefines.BLOCK_SIZE + i] = block.getData(i);
            }
            position++;
        }
        return position;
    }

    /**
     * Write the given buffer to the given disk starting at the given position
     *
     * @param disk the disk to write to
     * @param buffer the buffer containing the data to be written to disk
     * @param position the position in the disk where the data will be written.
     * @return The position of the last block written.
     */
    public static int writeBlocks(VirtualDisk disk, byte[] buffer, int position) throws IOException {
        int j;
        Block block = new Block();
        int nblock = Block.computeNBlock(buffer.length);
        for (j = 0; j < nblock; j++, position++){
            block.clear();
            for (int k = 3; k >= 0; k--) {
                block.setData(k, buffer[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, position);
        }
        if(buffer.length % OsDefines.BLOCK_SIZE != 0) {
            block.clear();
            for (int k = 0; k < (buffer.length%OsDefines.BLOCK_SIZE); k++) {
                block.setData(k, buffer[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, position);
            position++;
        }
        return position;
    }

    /**
     * Clear the data array
     */
    public void clear() {
        for (int i = 0; i < OsDefines.BLOCK_SIZE; i++) {
            data[i] = '\0';
        }
    }

    /**
     * Given a byte array, return a string representation of the byte array
     *
     * @return The string representation of the byte array.
     */
    public String toString(){
        StringBuilder str = new StringBuilder();
        for(byte b:this.data){
            if(b < 0x10 && b>=0){
                str.append(0);
            }
            str.append(String.format("%02X", b));
        }
        return str.toString();
    }
}
