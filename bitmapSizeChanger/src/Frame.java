import java.awt.Color;
import java.awt.Dimension;

import javax.swing.JFrame;

public class Frame extends JFrame {

	public static int WIDTH = 640;
	public static int HEIGHT = 360;

	String panelName;
	Panel panel;

	public Frame() {

		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setMinimumSize(new Dimension(WIDTH / 2, HEIGHT / 2));
		this.setSize(WIDTH, HEIGHT);
		this.setBackground(Color.BLACK);
		this.setVisible(true);
		this.setLocationRelativeTo(null);
		this.addPanel();
		this.setTitle("Change bitmap memory size");
		this.revalidate();

	}

	public void addPanel() {
		panel = new Panel(this);
		this.add(panel);
		this.revalidate();
	}

	public void maximize() {
		WIDTH = 1920;
		HEIGHT = 1080;
		this.setExtendedState(JFrame.MAXIMIZED_BOTH);
	}

	public void unmaximize() {
		WIDTH = 1280;
		HEIGHT = 720;
		this.setExtendedState(JFrame.NORMAL);
	}
}
