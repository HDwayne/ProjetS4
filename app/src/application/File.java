package application;

import java.io.IOException;

public class File {
    private int size;
    private byte[] data = new byte[OsDefines.MAX_FILE_SIZE];

    public File(){
    }

    public void setSize(int size){
        this.size = size;
    }

    public void setData(byte[] data) {
        if (this.size >= 0) System.arraycopy(data, 0, this.data, 0, this.size);
        if (this.size != OsDefines.MAX_FILE_SIZE){
            for (int i = data.length; i < OsDefines.MAX_FILE_SIZE; i++){
                this.data[i] = '\0';
            }
        }
    }

    public void eraseFromDisk(VirtualDisk disk, Inode inode) throws IOException {
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        Block block = new Block();
        block.clear();
        int nBlocks = this.size/OsDefines.BLOCK_SIZE;
        for (int j = 0; j < nBlocks; j++, i++){
            block.writeBlock(disk, i);
        }
        if (this.size%4 != 0){
            block.writeBlock(disk, i);
        }
    }

    public void write(VirtualDisk disk, Inode inode) throws IOException {
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        Block block = new Block();
        int nBlocks = this.size/OsDefines.BLOCK_SIZE;
        for (int j = 0; j < nBlocks; j++, i++){
            for (int k = 0; k < OsDefines.BLOCK_SIZE; k++){
                block.setData(k, this.data[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
        if (this.size%4 != 0){
            for (int k = 0; k < this.size%4; k++){
                block.setData(k, this.data[nBlocks*OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
    }

    public void read(VirtualDisk disk, Inode inode) throws IOException {
        this.setSize(inode.getSize());
        int i = inode.getFirstByte()/OsDefines.BLOCK_SIZE;
        Block block = new Block();
        byte[] buffer = new byte[this.size];
        int nBlocks = this.size/OsDefines.BLOCK_SIZE;
        System.out.println("size: " + this.size + " nBlocks: " + nBlocks + "total " + (nBlocks)*4 + (this.size%4));
        for (int j = 0; j < nBlocks; j++, i++){
            block.readBlock(disk, i);
            for (int k = 0; k < OsDefines.BLOCK_SIZE; k++){
                buffer[j * OsDefines.BLOCK_SIZE + k] = block.getData(k);
            }
        }
        if (this.size%4 != 0){
            block.readBlock(disk, i);
            for (int k = this.size%4-1; k >=0; k--){
                buffer[nBlocks*OsDefines.BLOCK_SIZE + k] = block.getData(k);
            }
        }
        this.setData(buffer);
    }


    public String dataToString(){
        String str = "";
        for (int i = 0; i < this.size; i++){
            str += (char)this.data[i];
        }
        return str;
    }

    public int getSize(){
        return this.size;
    }

    public byte[] getData(){
        return this.data;
    }

    public String toString(){
        String data = new String(this.data);
        data = data.replace("\0", "");
        return data;
    }
}
