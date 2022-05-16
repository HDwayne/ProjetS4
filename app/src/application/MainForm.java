package application;

import javax.swing.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;

public class MainForm extends JFrame{
    private JPanel panel;
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
    private JPanel JSysAnalyse;
    private JButton JButtonAnalyse;
    private JTextArea JTextAreaAS;
    private JButton JButtonRefreshF;
    private JButton JButtonRefreshIT;
    private JButton JButtonRefreshUT;
    private JButton JButtonRefreshSB;
    private JPanel JDefrag;
    private JButton JButtonDefrag;
    private JTextArea JTextAreaDefrag;
    private JScrollPane ScrollPanelSB;
    private JScrollPane ScrollPaneUT;
    private JScrollPane ScrollPaneInode;
    private JScrollPane ScrollPaneF;
    private JScrollPane ScrollPaneAS;
    private JScrollPane ScrollPaneDefrag;
    private JPanel mainPanel;
    private VirtualDisk vd = null;

    public MainForm() throws IOException {
        setContentPane(panel);
        setTitle("OADS");
        setSize(750,500);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setVisible(true);

        jButtonFolder.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);

                JFileChooser fileselect = new JFileChooser();
                fileselect.setCurrentDirectory(new File(System.getProperty("user.dir")));

                int response = fileselect.showOpenDialog(null);
                if (response == JFileChooser.APPROVE_OPTION){
                    JTextAreaAS.setText("");
                    JTextAreaF.setText("");
                    JTextAreaIT.setText("");
                    JTextAreaUT.setText("");
                    JTextAreaSB.setText("");
                    JTextAreaDefrag.setText("");
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
                    }
                }
            }
        });

        JButtonAnalyse.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);

                if (vd != null){
                    try {
                        JTextAreaAS.setText(vd.analysis());
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(null, "Une erreur est survenue lors de l'analyse");
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });

        JButtonRefreshF.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (vd != null) {
                    JTextAreaF.setText("");
                    for (int i = 0; i < OsDefines.INODE_TABLE_SIZE; i++) {
                        application.File file = new application.File();
                        if (!vd.getInode(i).isFree()) {
                            try {
                                file.read(vd, vd.getInode(i));
                            } catch (IOException ex) {
                                throw new RuntimeException(ex);
                            }
                        }
                        JTextAreaF.append("File " + i + ": " + new String(vd.getInode(i).getFileName()) + "\n");
                        JTextAreaF.append(file + "------------------------------\n");
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });

        JButtonRefreshIT.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (vd != null){
                    JTextAreaIT.setText(vd.getInodeTableString());
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });

        JButtonRefreshUT.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (vd != null) {
                    JTextAreaUT.setText(vd.getUserTableString());
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });

        JButtonRefreshSB.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (vd != null){
                    JTextAreaSB.setText(vd.getSuperBlockString());
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });

        JButtonDefrag.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (vd != null){
                    try {
                        JTextAreaDefrag.setText(vd.defragmentation());
                        vd.saveDisk();
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(null, "Une erreur est survenue lors de la défragmentation");
                    }
                } else {
                    JOptionPane.showMessageDialog(null, "Merci de sélectionner un disk");
                }
            }
        });
    }

    public static void main(String[] args) throws IOException {
        MainForm form = new MainForm();
    }
}