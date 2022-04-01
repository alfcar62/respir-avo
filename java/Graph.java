import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Graph {

    JFrame canvas;
    private List<Point> data;
    private BufferedReader file;
    private List<String> heading;
    private int[] fieldIndexes;
    private Point center;
    private int zoom;


    public Graph(String srcFile, String fieldY, int centerX, int centerY, int zoom) {
        super();

        try {
            this.file = new BufferedReader(new FileReader(srcFile));
            this.center = new Point(centerX, centerY);
            this.zoom = zoom;

            this.checkHeading(fieldY);

            this.loadData();

            this.startUpCanvas();

            /* Debug
            for (Point point : this.data) {
                System.out.println(point.getX() + " : " + point.getY());
            }

            for (String point : this.heading) {
                System.out.print(point + "|");
            }
            */
        } catch (Exception e) {
            System.out.println("errore");
        }
    }

    private void checkHeading(String fieldY) throws IOException {
        this.heading = Arrays.asList(this.file.readLine().split(","));
        this.fieldIndexes = new int[]{this.heading.indexOf("timestamp"), this.heading.indexOf(fieldY)};

        if(this.fieldIndexes[0] == -1 || this.fieldIndexes[1] == -1)
            throw new IOException("Campi non presenti nel intestazione");
    }

    private void loadData() throws IOException {
        this.data = new ArrayList<Point>();
        String line;

        while((line = file.readLine()) != null) {
            String[] splittedLine = line.split(",");
            this.data.add(new Point( (Double.valueOf( splittedLine[this.fieldIndexes[0]] ) - 1.645566955E9) / 60,
                                     Double.valueOf(splittedLine[this.fieldIndexes[1]])));
        }

    }

    private void startUpCanvas() {
        this.canvas = new JFrame();

        canvas.add(new Canvas(this.data, new Point(40, 800), 5, 2, 6));

        canvas.setSize(1450,900);
        canvas.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        canvas.setVisible(true);
    }

    private class Point {
        private double x;
        private double y;

        public Point(double x, double y) {
            this.x = x;
            this.y = y;
        }

        public double getX() { return this.x; }
        public double getY() { return this.y; }

        public void setX(int x) { this.x = x; }
        public void setY(int y) { this.y = y; }
    }

    public class Canvas extends JComponent {

        private List<Point> points;
        private Point center;
        private int zoom;
        private double stepX;
        private double stepY;

        public Canvas(List<Point> points, Point center, int zoom, double stepX, double stepY) {
            super();

            this.points = points;
            this.center = center;
            this.zoom = zoom;
            this.stepX = stepX;
            this.stepY = stepY;
        }

        public Canvas(List<Point> points, Point center, int zoom) { this(points, center, zoom, 1, 1); }

        public Canvas(List<Point> points, Point center) { this(points, center, 1, 1, 1); }

        public Canvas(List<Point> points) { this(points, new Point(20, 850), 1, 1, 1); }

        @Override
        public void paint(Graphics g) {

            //_______print-axis___________
            g.drawLine((int) this.center.getX(),10 * (-this.zoom), (int) this.center.getX(), 900);
            g.drawLine(10, (int) this.center.getY(), 180000 * this.zoom, (int) this.center.getY());

            for(int i = 0; i <= 900; i += this.stepX) {
                g.drawLine((int) this.center.getX() - 4,
                          (int) this.center.getY() - i * this.zoom,
                          (int) this.center.getX() + 4,
                          (int) this.center.getY() - i * this.zoom);
            }

            for(int i = 0; i <= 180000; i += this.stepY) {
                g.drawLine((int) this.center.getX() + i * this.zoom,
                        (int) this.center.getY() - 4,
                        (int) this.center.getX() + i * this.zoom,
                        (int) this.center.getY() + 4);
            }

            //_______print-graph__________
            for(int i = 0; i < this.points.size() - 1; i++) {
                g.drawLine( ( (int) this.center.getX() + (int) this.points.get(i).getX() * this.zoom ) ,
                            ( (int) this.center.getY() - (int) this.points.get(i).getY() * this.zoom ) ,
                            ( (int) this.center.getX() + (int) this.points.get(i + 1).getX() * this.zoom ) ,
                            ( (int) this.center.getY() - (int) this.points.get(i + 1).getY() * this.zoom ) );
            }

        }
    }
}
