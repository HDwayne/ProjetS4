package application;


import java.io.IOException;

/**
 * This class is used to store the information of the inode
 */
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

    // This is the constructor of the class Inode. It is used to initialize the inode.
    public Inode(){
        this.setFileName("\0".getBytes());
        size = 0;
        nBlock = 0;
        firstByte = 0;
    }

    /**
     * This function is used to initialize the file information
     *
     * @param fileName the name of the file
     * @param size the size of the file in bytes
     * @param uId the user id of the owner of the file
     * @param uRight the user's right to the file
     * @param oRight the owner's right to read/write the file
     * @param cTimeStamp creation time stamp
     * @param mTimeStamp the last modified time stamp
     * @param nBlock the number of blocks in the file
     * @param firstByte the first byte of the file
     */
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

    /**
     * This function sets the file name of the file
     *
     * @param fileName The name of the file to be created.
     */
    public void setFileName(byte[] fileName){
        int i;
        for (i = 0; i < fileName.length && i < OsDefines.FILENAME_MAX_SIZE; i++){
            this.fileName[i] = fileName[i];
        }
        for(; i < OsDefines.FILENAME_MAX_SIZE; i++){
            this.fileName[i] = '\0';
        }
    }

    /**
     * It sets the size of the file.
     *
     * @param size The size of the file.
     */
    public void setSize(int size){
        this.size = size;
    }

    /**
     * It sets the user id of the user.
     *
     * @param uId The user id of the user who is owner of the file.
     */
    public void setuId(int uId){
        this.uId = uId;
    }

    /**
     * It sets the value of uRight to the value of uRight.
     *
     * @param uRight The right of the user on the file.
     */
    public void setuRight(int uRight){
        this.uRight = uRight;
    }

    /**
     * It sets the value of the oRight variable to the value of the parameter passed in.
     *
     * @param oRight The right of other users on the file.
     */
    public void setoRight(int oRight){
        this.oRight = oRight;
    }

    /**
     * This function sets the cTimeStamp field of the inode to the value of the cTimeStamp parameter
     *
     * @param cTimeStamp The time stamp of the file.
     */
    public void setcTimeStamp(byte[] cTimeStamp){
        int i;
        for (i = 0; i < cTimeStamp.length && i < OsDefines.TIMESTAMP_SIZE; i++){
            this.cTimeStamp[i] = cTimeStamp[i];
        }
        for(; i < OsDefines.TIMESTAMP_SIZE; i++){
            this.cTimeStamp[i] = '$';
        }
    }

    /**
     * This function sets the mTimeStamp field of the class to the value of the mTimeStamp parameter
     *
     * @param mTimeStamp The time stamp of the file.
     */
    public void setmTimeStamp(byte[] mTimeStamp){
        int i;
        for (i = 0; i < mTimeStamp.length && i < OsDefines.TIMESTAMP_SIZE; i++){
            this.mTimeStamp[i] = mTimeStamp[i];
        }
        for(; i < OsDefines.TIMESTAMP_SIZE; i++){
            this.mTimeStamp[i] = '$';
        }
    }

    /**
     * It sets the value of nBlock to the value of the parameter nBlock.
     *
     * @param nBlock The number of blocks.
     */
    public void setnBlock(int nBlock){
        this.nBlock = nBlock;
    }

    /**
     * It sets the first byte of the file.
     *
     * @param firstByte The first byte of the file.
     */
    public void setFirstByte(int firstByte){
        this.firstByte = firstByte;
    }

    /**
     * Given a file name, return the file name in bytes
     *
     * @return The file name as a byte array.
     */
    public byte[] getFileName(){
        String filename = new String(this.fileName);
        filename = filename.replace("\0", "");
        return filename.getBytes();
    }

    /**
     * Returns the size of the file
     *
     * @return The size of the file.
     */
    public int getSize(){
        return this.size;
    }

    /**
     * It removes the null characters from the string.
     *
     * @return The cTimeStamp string.
     */
    public byte[] getcTimeStamp(){
        String cTimeStamp = new String(this.cTimeStamp);
        cTimeStamp = cTimeStamp.replace("\0", "");
        return cTimeStamp.getBytes();
    }

    /**
     * This function returns the mTimeStamp field of the file as a byte array
     *
     * @return The timestamp in bytes.
     */
    public byte[] getmTimeStamp(){
        String mTimeStamp = new String(this.mTimeStamp);
        mTimeStamp = mTimeStamp.replace("\0", "");
        return mTimeStamp.getBytes();
    }

    /**
     * Returns the number of blocks of the current file
     *
     * @return The number of blocks.
     */
    public int getnBlock(){
        return this.nBlock;
    }

    /**
     * Returns the first byte of the file
     *
     * @return The first byte of the file.
     */
    public int getFirstByte(){
        return this.firstByte;
    }

    /**
     * This function is used to print the file information in a human readable format
     *
     * @return The string representation of the file metadata.
     */
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

    /**
     * Return this.firstByte == 0;
     *
     * @return A boolean value.
     */
    public boolean isFree(){
        return this.firstByte == 0;
    }

    /**
     * Write the file's metadata to disk
     *
     * @param disk the disk to write to
     * @param inodeId the inode number of the file
     */
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

    /**
     * Reads the inode from the disk and initializes the inode
     *
     * @param disk the disk to read from
     * @param inodeId the inode number of the file
     */
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

    public String analysis(VirtualDisk disk, int inodeId){
        String result = "Début d'analyse de l'inode n°" + inodeId + "\n";

        int nBlockFound = this.size % OsDefines.BLOCK_SIZE == 0 ? this.size / OsDefines.BLOCK_SIZE : this.size / OsDefines.BLOCK_SIZE + 1;
        if (nBlockFound != this.nBlock) {
            result += "Le nombre de blocs de l'inode n'est pas correct " + this.nBlock + " != " + nBlockFound + "\n";
        }else{
            result += "Le nombre de blocs de l'inode est correct " + this.nBlock + " == " + nBlockFound + "\n";
        }

        if (this.uRight == 0 ){
            result += "L'inode n'est pas accessible en lecture par celui qui possède le fichier.\n";
        }

        if (this.oRight > this.uRight ){
            result += "Le propriétaire du fichier possède des droits inférieurs aux autre utilisateurs.\n";
        }

        // Convert the byte array timestamp to a date
        int nextInode = this.firstByte + this.nBlock*OsDefines.BLOCK_SIZE;
        if (disk.getInode(inodeId + 1).isFree()){
            if (SuperBlock.getFirstFreeByte() != nextInode){
                result += "Le prochain inode est libre mais ne sera pas initialisé au bon endroit sur le disk. First free byte = " +  SuperBlock.getFirstFreeByte() + " Valeur calculée : " + nextInode + "\n";
            }else{
                result += "Le prochain inode est libre et sera initialisé au bon endroit sur le disk.\n";
            }
        }else{
            if(nextInode != disk.getInode(inodeId + 1).getFirstByte()){
                result += "Problème de fragmentation. La prochaine inode est déjà occupée mais n'est pas initialisée au bon endroit sur le disk. First byte = " +  disk.getInode(inodeId + 1).getFirstByte() + " Valeur calculée : " + nextInode + "\n";
            }else{
                result += "La prochaine inode est déjà occupée et est initialisée au bon endroit sur le disk.\n";
            }
        }

        return result;
    }
}
