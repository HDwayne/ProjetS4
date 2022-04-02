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
        i = Block.writeBlocks(disk, this.fileName, i);

        block.fromInt(this.size);
        block.writeBlock(disk, i);
        i++;

        block.fromInt(this.uId);
        block.writeBlock(disk, i);
        i++;

        block.fromInt(this.uRight);
        block.writeBlock(disk, i);
        i++;

        block.fromInt(this.oRight);
        block.writeBlock(disk, i);
        i++;

        i = Block.writeBlocks(disk, this.cTimeStamp, i);
        i = Block.writeBlocks(disk, this.mTimeStamp, i);

        block.fromInt(this.nBlock);
        block.writeBlock(disk, i);
        i++;

        block.fromInt(this.firstByte);
        block.writeBlock(disk, i);
    }

    public void read(VirtualDisk disk, int inodeId) throws IOException {
        Block block = new Block();

        byte[] filename = new byte[OsDefines.FILENAME_MAX_SIZE];
        int i = OsDefines.INODES_START/OsDefines.BLOCK_SIZE + inodeId * OsDefines.INODE_SIZE;
        i = Block.readBlocks(disk, filename, i);

        block.readBlock(disk,i);
        int size = block.toInt();
        i++;

        block.readBlock(disk,i);
        int uId = block.toInt();
        i++;

        block.readBlock(disk,i);
        int uRight = block.toInt();
        i++;

        block.readBlock(disk,i);
        int oRight = block.toInt();
        i++;

        byte[] cTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
        i = Block.readBlocks(disk, cTimeStamp, i);

        byte[] mTimeStamp = new byte[OsDefines.TIMESTAMP_SIZE];
        i = Block.readBlocks(disk, mTimeStamp, i);

        block.readBlock(disk,i);
        int nBlock = block.toInt();
        i++;

        block.readBlock(disk,i);
        int firstByte = block.toInt();

        this.init(filename, size, uId, uRight, oRight, cTimeStamp, mTimeStamp, nBlock, firstByte);
    }
}
