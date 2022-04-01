package application;

import java.io.*;

public class Inode {
    private byte[] fileName = new byte[OsDefines.FILENAME_MAX_SIZE];
    private int size;
    private int uId;
    private int uRight;
    private int oRight;
    private byte[] cTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
    private byte[] mTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
    private int nBlock;
    private int firstByte;

    public Inode(){
        this.setFileName("\0".getBytes());
        size = 0;
        nBlock = 0;
        firstByte = 0;
    }

    public void init(byte[] fileName, int size, int uId, int uRight, int oRight, byte[] cTimeStamp, byte[] mTimeStamp, int nBlock, int firstByte){
        this.setFileName(fileName);
        this.setSize(size);
        this.setuId(uId);
        this.setuRight(uRight);
        this.setoRight(oRight);
        this.setcTimeStamp(cTimeStamp);
        this.setmTimeStamp(mTimeStamp);
        this.setnBlock(nBlock);
        this.setFirstByte(firstByte);
    }

    public void deleteInode(){
        this.init("\0".getBytes(), 0, 0, 0, 0, "\0".getBytes(), "\0".getBytes(), 0, 0);
    }

    public void setFileName(byte[] fileName){
        int i;
        for (i = 0; i < fileName.length && i < OsDefines.FILENAME_MAX_SIZE; i++){
            this.fileName[i] = fileName[i];
        }
        for(; i < OsDefines.FILENAME_MAX_SIZE; i++){
            this.fileName[i] = '\0';
        }
    }

    public void setSize(int size){
        this.size = size;
    }

    public void setuId(int uId){
        this.uId = uId;
    }

    public void setuRight(int uRight){
        this.uRight = uRight;
    }

    public void setoRight(int oRight){
        this.oRight = oRight;
    }

    public void setcTimeStamp(byte[] cTimeStamp){
        int i;
        for (i = 0; i < cTimeStamp.length && i < OsDefines.TIMESTAMP_SIZE; i++){
            this.cTimeStamp[i] = cTimeStamp[i];
        }
        for(; i < OsDefines.TIMESTAMP_SIZE; i++){
            this.cTimeStamp[i] = '$';
        }
    }

    public void setmTimeStamp(byte[] mTimeStamp){
        int i;
        for (i = 0; i < mTimeStamp.length && i < OsDefines.TIMESTAMP_SIZE; i++){
            this.mTimeStamp[i] = mTimeStamp[i];
        }
        for(; i < OsDefines.TIMESTAMP_SIZE; i++){
            this.mTimeStamp[i] = '$';
        }
    }

    public void setnBlock(int nBlock){
        this.nBlock = nBlock;
    }

    public void setFirstByte(int firstByte){
        this.firstByte = firstByte;
    }

    public byte[] getFileName(){
        String filename = new String(this.fileName);
        filename = filename.replace("\0", "");
        return filename.getBytes();
    }

    public int getSize(){
        return this.size;
    }

    public int getuId(){
        return this.uId;
    }

    public int getuRight(){
        return this.uRight;
    }

    public int getoRight(){
        return this.oRight;
    }

    public byte[] getcTimeStamp(){
        String cTimeStamp = new String(this.cTimeStamp);
        cTimeStamp = cTimeStamp.replace("\0", "");
        return cTimeStamp.getBytes();
    }

    public byte[] getmTimeStamp(){
        String mTimeStamp = new String(this.mTimeStamp);
        mTimeStamp = mTimeStamp.replace("\0", "");
        return mTimeStamp.getBytes();
    }

    public int getnBlock(){
        return this.nBlock;
    }

    public int getFirstByte(){
        return this.firstByte;
    }

    public String toString(){
        String str = "";
        str += "fileName: " + new String(this.getFileName()) + "\n";
        str += "size: " + this.size + "\n";
        str += "uId: " + this.uId + "\n";
        str += "uRight: " + this.uRight + "\n";
        str += "oRight: " + this.oRight + "\n";
        str += "cTimeStamp: " + new String(this.getcTimeStamp()) + "\n";
        str += "mTimeStamp: " + new String(this.getmTimeStamp()) + "\n";
        str += "nBlock: " + this.nBlock + "\n";
        str += "firstByte: " + this.firstByte + "\n";
        return str;
    }

    public boolean isFree(){
        return this.firstByte == 0;
    }

    public void write(VirtualDisk disk, int inodeId) throws IOException {
        Block block = new Block();

        int i = OsDefines.INODES_START/OsDefines.BLOCK_SIZE + inodeId * OsDefines.INODE_SIZE;
        for (int j = 0; j < Block.computeNBlock(OsDefines.FILENAME_MAX_SIZE); j++, i++){
            for (int k = 3; k >= 0; k--) {
                block.setData(k, this.fileName[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }

        for (int j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.size >> (j * 8)));  //size
        }
        block.writeBlock(disk, i);
        i++;

        for (int j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.uId >> (j * 8)));
        }
        block.writeBlock(disk, i);
        i++;

        for (int j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.uRight >> (j * 8)));
        }
        block.writeBlock(disk, i);
        i++;

        for (int j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.oRight >> (j * 8)));
        }
        block.writeBlock(disk, i);
        i++;

        int j;
        for (j = 0; j < Block.computeNBlock(OsDefines.TIMESTAMP_SIZE)-1; j++, i++){
            for (int k = 3; k >= 0; k--) {
                block.setData(k, this.cTimeStamp[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
        for (int k = 1; k >= 0; k--) {
            block.setData(k, cTimeStamp[j*OsDefines.BLOCK_SIZE + k]);
        }
        block.setData(3, (byte)'\0');
        block.setData(2, (byte)'\0');
        block.writeBlock(disk, i);
        i++;


        for (j = 0; j < Block.computeNBlock(OsDefines.TIMESTAMP_SIZE)-1; j++, i++){
            for (int k = 3; k >= 0; k--) {
                block.setData(k, this.mTimeStamp[j * OsDefines.BLOCK_SIZE + k]);
            }
            block.writeBlock(disk, i);
        }
        for (int k = 1; k >= 0; k--) {
            block.setData(k, mTimeStamp[j*OsDefines.BLOCK_SIZE + k]);
        }
        block.setData(3, (byte)'\0');
        block.setData(2, (byte)'\0');
        block.writeBlock(disk, i);
        i++;

        for (j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.nBlock >> (j * 8)));
        }
        block.writeBlock(disk, i);
        i++;

        for (j = 3; j >= 0; j--) {
            block.setData(j, (byte) (this.firstByte >> (j * 8)));
        }
        block.writeBlock(disk, i);
    }

    public void read(VirtualDisk disk, int inodeId) throws IOException {
        Block block = new Block();

        byte[] filename = new byte[OsDefines.FILENAME_MAX_SIZE];
        int i = OsDefines.INODES_START/OsDefines.BLOCK_SIZE + inodeId * OsDefines.INODE_SIZE;
        for (int k = 0; k < Block.computeNBlock(OsDefines.FILENAME_MAX_SIZE); k++, i++) {
            block.readBlock(disk, i);
            for (int j = 3; j >= 0; j--) {
                filename[k*OsDefines.BLOCK_SIZE + j] = block.getData(j);
            }
        }

        int size = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            size = (size << 8) + (block.getData(j) & 0xFF);
        }
        i++;

        int uId = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            uId = (uId << 8) + (block.getData(j) & 0xFF);
        }
        i++;

        int uRight = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            uRight = (uRight << 8) + (block.getData(j) & 0xFF);
        }
        i++;

        int oRight = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            oRight = (oRight << 8) + (block.getData(j) & 0xFF);
        }
        i++;

        byte[] cTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
        int k;
        for (k = 0; k < Block.computeNBlock(OsDefines.TIMESTAMP_SIZE)-1; k++, i++) {
            block.readBlock(disk, i);
            for (int j = 3; j >= 0; j--) {
                cTimeStamp[k * OsDefines.BLOCK_SIZE + j] = block.getData(j);
            }
        }
        block.readBlock(disk, i);
        for (int j = 1; j >= 0; j--) {
            cTimeStamp[k*OsDefines.BLOCK_SIZE + j] = block.getData(j);
        }
        i++;


        byte[] mTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
        for (k = 0; k < Block.computeNBlock(OsDefines.TIMESTAMP_SIZE)-1; k++, i++) {
            block.readBlock(disk, i);
            for (int j = 3; j >= 0; j--) {
                mTimeStamp[k*OsDefines.BLOCK_SIZE + j] = block.getData(j);
            }
        }
        block.readBlock(disk, i);
        for (int j = 1; j >= 0; j--) {
            mTimeStamp[k*OsDefines.BLOCK_SIZE] = block.getData(j);
        }
        i++;

        int nBlock = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            nBlock = (nBlock << 8) + (block.getData(j) & 0xFF);
        }
        i++;

        int firstByte = 0;
        block.readBlock(disk,i);
        for (int j = 3; j >= 0; j--){
            firstByte = (firstByte << 8) + (block.getData(j) & 0xFF);
        }

        this.init(filename, size, uId, uRight, oRight, cTimeStamp, mTimeStamp, nBlock, firstByte);
    }
}
