package application;

import java.io.IOException;

public class Block {
    private byte[] data = new byte[OsDefines.BLOCK_SIZE];

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
        return size / OsDefines.BLOCK_SIZE + (size % OsDefines.BLOCK_SIZE == 0 ? 0 : 1);
    }

    public void writeBlock(VirtualDisk disk, int position) throws IOException {
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        disk.getStorage().write(this.data);
    }

    public void readBlock(VirtualDisk disk, int position) throws IOException {
        disk.getStorage().seek((long) position * OsDefines.BLOCK_SIZE);
        for (int i = 0; i < OsDefines.BLOCK_SIZE; i++) {
            data[i] = disk.getStorage().readByte();
        }
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
