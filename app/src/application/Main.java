package application;

import java.io.IOException;

public class Main {
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        System.out.println("Working Directory = " + System.getProperty("user.dir"));
        VirtualDisk vd = new VirtualDisk();
        System.out.println(vd.toString());
        vd.saveDisk();

        for (int i = 0; i < OsDefines.INODE_TABLE_SIZE; i++) {
            File file = new File();
            if(!vd.getInode(i).isFree()) {
                file.read(vd, vd.getInode(i));
            }
            System.out.println("File " + i + ": " + new String(vd.getInode(i).getFileName()));
            System.out.println(file.toString());
            file.eraseFromDisk(vd, vd.getInode(i));
            file.write(vd, vd.getInode(i));
        }

        vd.defragmentation();
        vd.saveDisk();



    }
}
