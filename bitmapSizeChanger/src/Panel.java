import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class Panel extends JPanel {

    Frame frame;

    JButton buttonSelect;

    JTextArea output;

    JFileChooser fileChooser;

    BufferedImage image;

    public Panel(Frame frame) {

        this.frame = frame;
        this.setSize(Frame.WIDTH, Frame.HEIGHT);
        this.setLocation(0, 0);
        this.setFocusable(true);
        this.setBackground(Color.ORANGE);
        this.setLayout(new BorderLayout());

        buttonSelect = new JButton();
        createButton("Select image", buttonSelect);
        buttonSelect.addActionListener(e -> {
            inputImage();
        });

        output = new JTextArea();
        createTextArea(output);

        JScrollPane scroll = new JScrollPane(output);
        this.add(scroll);
    }

    public void createButton(String text, JButton button) {

        button.setFont(new Font("Arial", Font.BOLD, 12));
        button.setText(text);
        button.setFocusPainted(false);
        button.setOpaque(true);
        button.setVisible(true);
        button.setBackground(Color.WHITE);
        this.add(button, BorderLayout.NORTH);
    }

    public void createTextArea(JTextArea area) {
        area.setText("Here you will have the output");
        area.setOpaque(true);
        area.setVisible(true);
        area.setBackground(Color.WHITE);
        area.setLineWrap(true);
        area.setWrapStyleWord(true);
        this.add(area, BorderLayout.CENTER);
    }

    public void inputImage() {
        JFileChooser chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File("C:\\Users\\20241875\\OneDrive - TU Eindhoven\\Desktop"));
        int result = chooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            File file = chooser.getSelectedFile();
            try {
                BufferedImage image = ImageIO.read(file);
                // System.out.println(file.getPath());
                readImage(file.getPath());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void readImage(String path) {
        try {
            image = ImageIO.read(new File(path));
        } catch (IOException e) {
            e.printStackTrace();
        }
        output.setText("");
        String hex = "";
        String byteCode = "";
        int i = 0;
        for (int y = 0; y < image.getHeight(); y++) {
            for (int x = 0; x < image.getWidth(); x++) {
                int rgb = image.getRGB(x, y);
                int r = (rgb >> 16) & 0xFF;
                int g = (rgb >> 8) & 0xFF;
                int b = rgb & 0xFF;
                hex = String.format("#%02X%02X%02X", r, g, b);
                System.out.print(hex);
                if (i % 4 == 0)
                    byteCode = byteCode + ", 0b";
                if (hex.equals("#FFFFFF"))
                    byteCode = byteCode + "01";
                else if (hex.equals("#1F102A"))
                    byteCode = byteCode + "10";
                else
                    byteCode = byteCode + "00";
                i++;
            }
        }
        output.append(byteCode);
        System.out.println(byteCode);
    }

}
