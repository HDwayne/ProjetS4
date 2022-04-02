package application;

import java.io.IOException;

public class Block {
    private final byte[] data = new byte[OsDefines.BLOCK_SIZE];

    public Block(){
        this.clear();
    }

    public byte getData(int index) {
        return data[index];
    }

    public void setData(int index, byte value) {
        data[index] = value;
    }

    public static int computeNBlock(int size) {
        return size / OsDefines.BLOCK_SIZE;
    }

    public void writeBlock(VirtualDisk disk, int position) throws IOException {
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        disk.getStorage().write(this.data);
    }

    public void readBlock(VirtualDisk disk, int position) throws IOException {
        this.clear();
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        for (int i = 0; i < OsDefines.BLOCK_SIZE; i++) {
            data[i] = disk.getStorage().readByte();
        }
    }

    public int toInt(){
        int n = 0;
        for(int i=OsDefines.BLOCK_SIZE-1;i>=0;i--){
            n = (n << 8) + (data[i] & 0xff);
        }
        return n;
    }

    public void fromInt(int n){
        this.clear();
        for(int i=OsDefines.BLOCK_SIZE - 1;i >= 0;i--){
            this.setData(i, (byte) ((n >> (i * 8)) & 0xFF));
        }
    }

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

    public void clear() {
        for (int i = 0; i < OsDefines.BLOCK_SIZE; i++) {
            data[i] = '\0';
        }
    }

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
