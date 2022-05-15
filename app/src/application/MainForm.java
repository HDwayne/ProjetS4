package application;

import javax.swing.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;

public class MainForm extends JFrame{
    private JPanel Panel1;
    private JButton jButtonFolder;
    private JLabel jLabelFolder;
    private JTabbedPane tabbedPane1;
    private JPanel jUserTable;
    private JPanel jFolders;
    private JPanel JInodeTable;
    private JPanel JSuperBlock;
    private JTextArea JTextAreaSB;
    private JTextArea JTextAreaIT;
    private JTextArea JTextAreaF;
    private JTextArea JTextAreaUT;

    public MainForm() throws IOException {
        setContentPane(Panel1);
        setTitle("oui");
        setSize(400,300);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);

        jButtonFolder.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);

                JFileChooser fileselect = new JFileChooser();
                fileselect.setCurrentDirectory(new File(System.getProperty("user.dir")));

                int response = fileselect.showSaveDialog(null);
                if (response == JFileChooser.APPROVE_OPTION){
                    // File file = new File(fileselect.getSelectedFile().getAbsolutePath());

                    VirtualDisk vd = null;
                    try {
                        vd = new VirtualDisk(fileselect.getSelectedFile().getAbsolutePath());
                        JTextAreaSB.setText(vd.getSuperBlockString());
                        JTextAreaUT.setText(vd.getUserTableString());
                        JTextAreaIT.setText(vd.getInodeTableString());
                        JTextAreaF.setText("");
                        for (int i = 0; i < OsDefines.INODE_TABLE_SIZE; i++) {
                            application.File file = new application.File();
                            if(!vd.getInode(i).isFree()) {
                                file.read(vd, vd.getInode(i));
                            }
                            JTextAreaF.append("File " + i + ": " + new String(vd.getInode(i).getFileName()) + "\n");
                            JTextAreaF.append(file.toString() + "------------------------------\n");
                        }

                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(null, "Fichier sélectionné invalide");
                        // throw new RuntimeException(ex);
                    }
                }
            }
        });


        /*
        System.out.println("Début défragmentation");
        vd.defragmentation();
        vd.saveDisk();
        */
    }

    public static void main(String[] args) throws IOException {
        MainForm form = new MainForm();
    }
}
