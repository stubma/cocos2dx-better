import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.filechooser.FileNameExtensionFilter;

public class Main {
	public static void main(String[] args) throws Exception {
		// window frame
		JFrame f = new JFrame("Excel Exporter");
		Container container = f.getContentPane();
		container.setLayout(new GridLayout(2, 1));
		
		// help area
		StringBuilder buf = new StringBuilder("1. first row can be human readable column name\n");
		buf.append("2. second row is reversed and not used now\n");
		buf.append("3. third row should be English name of column, first letter will be automatically converted to upppercase\n");
		buf.append("4. fourth row is case-insensitive column type name, can be bool, byte, int, string, float, stringarray, intarray, boolarray, floatarray and luafunc\n");
		buf.append("5. first column must be a primary key column\n");
		buf.append("6. if you don't want a column to be generated, just leave column name or type blank");
		JTextArea t = new JTextArea(buf.toString());
		t.setEditable(false);
		t.setBackground(f.getBackground());
		t.setBorder(BorderFactory.createTitledBorder("Help"));
		container.add(t);
		
		// bottom layout
		Container bottom = new Container();
		bottom.setLayout(new GridLayout(1, 2));
		container.add(bottom);

		// add combo
		String[] s = { "Excel to Json/C++" };
		final JComboBox cmb = new JComboBox(s);
		cmb.setBorder(BorderFactory.createTitledBorder("Export Type"));
		bottom.add(cmb);

		// add button
		JButton btn = new JButton("Select Excel Files");
		btn.setBackground(Color.GREEN);
		bottom.add(btn);

		// show window
		f.pack();
		f.setVisible(true);

		// set window location
		Dimension winSize = Toolkit.getDefaultToolkit().getScreenSize();
		f.setLocation((winSize.width - f.getWidth()) / 2, (winSize.height - f.getHeight()) / 2);

		// handle button
		btn.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent evt) {
				switch (cmb.getSelectedIndex()) {
					case 0:
					{
						ExcelToJson export = new ExcelToJson();
						JFileChooser fileChooser = new JFileChooser(System.getProperty("user.home"));
						FileNameExtensionFilter filter = new FileNameExtensionFilter("Excel Files", "xls", "xlsx");
						fileChooser.setMultiSelectionEnabled(true);
						fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
						fileChooser.setFileFilter(filter);
						int returnVal = fileChooser.showOpenDialog(fileChooser);
						if (returnVal == JFileChooser.APPROVE_OPTION) {
							File files[] = fileChooser.getSelectedFiles();
							export.start(files);
						}
						break;
					}
				}
			}
		});
	}
}
