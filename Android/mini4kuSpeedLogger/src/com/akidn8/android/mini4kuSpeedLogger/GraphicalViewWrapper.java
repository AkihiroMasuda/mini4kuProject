package com.akidn8.android.mini4kuSpeedLogger;

import java.util.ArrayList;
import java.util.List;

import org.achartengine.ChartFactory;
import org.achartengine.GraphicalView;
import org.achartengine.chart.AbstractChart;
import org.achartengine.chart.PointStyle;
import org.achartengine.model.XYMultipleSeriesDataset;
import org.achartengine.model.XYSeries;
import org.achartengine.renderer.XYMultipleSeriesRenderer;
import org.achartengine.renderer.XYSeriesRenderer;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Paint.Align;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Handler;


public class GraphicalViewWrapper{
	
	private XYMultipleSeriesDataset dataset;
	private GraphicalView graphicalView;
	XYMultipleSeriesRenderer ren = null;
	
	public GraphicalViewWrapper(Context context) {
		
		String[] titles = new String[] { "Blue" };
		List<double[]> x = new ArrayList<double[]>();
		for (int i = 0; i < titles.length; i++) {
//			x.add(new double[] { 1, 2, 3, 4, 5 });
			x.add(new double[] {});
		}
		List<double[]> values = new ArrayList<double[]>();
//		values.add(new double[] { 1, 2, 3, 4, 5 });
//		values.add(new double[] { 18, 17, 16, 15, 14 });
		values.add(new double[] {  });
		values.add(new double[] {  });
		int[] colors = new int[] { Color.BLUE };
//		PointStyle[] styles = new PointStyle[] { PointStyle.POINT };
		PointStyle[] styles = new PointStyle[] { PointStyle.CIRCLE };
		XYMultipleSeriesRenderer renderer = buildRenderer(colors, styles);
		ren = renderer;
		int length = renderer.getSeriesRendererCount();
		for (int i = 0; i < length; i++) {
			((XYSeriesRenderer) renderer.getSeriesRendererAt(i))
					.setFillPoints(true);
		}
		double xmin = 0.5;
		double xmax = 12.5;
		double ymin = 0.0;
		double ymax = 40.0;
		setChartSettings(renderer, "Average temperature", "Horizontal axis",
				"Vertical axis", xmin, xmax, ymin, ymax, Color.LTGRAY, Color.BLUE);
		renderer.setXLabels(12);
		renderer.setYLabels(10);
		renderer.setShowGrid(true);
		renderer.setXLabelsAlign(Align.RIGHT);
		renderer.setYLabelsAlign(Align.RIGHT);
		renderer.setZoomButtonsVisible(true);
		renderer.setGridColor(Color.BLACK);
		
/*
 		renderer.setPanLimits(new double[] { -10, 20, -10, 40 });
		renderer.setZoomLimits(new double[] { -10, 20, -10, 40 });
*/
		dataset = buildDataset(titles, x, values);
		
		graphicalView = ChartFactory.getLineChartView(
				context, dataset, renderer);

	}
	
	public GraphicalView getView(){
		return graphicalView;
	}
	
	public void add(int index, double x, double y){
		dataset.getSeriesAt(index).add(x, y);
	}
	
	public void setXAxis(double min, double max){
		ren.setXAxisMin(min);
		ren.setXAxisMax(max);
	}
	
	public void setYAxis(double min, double max){
		ren.setYAxisMin(min);
		ren.setYAxisMax(max);
	}
	
	public void repaint(){
		graphicalView.repaint();
	}
	
	public void setTitle(String title){
		ren.setChartTitle(title);
	}
	
	
	private XYMultipleSeriesDataset buildDataset(String[] titles,
			List<double[]> xValues, List<double[]> yValues) {
		XYMultipleSeriesDataset dataset = new XYMultipleSeriesDataset();
		addXYSeries(dataset, titles, xValues, yValues, 0);
		return dataset;
	}

	private void addXYSeries(XYMultipleSeriesDataset dataset, String[] titles,
			List<double[]> xValues, List<double[]> yValues, int scale) {
		int length = titles.length;
		for (int i = 0; i < length; i++) {
			XYSeries series = new XYSeries(titles[i], scale);
			double[] xV = xValues.get(i);
			double[] yV = yValues.get(i);
			int seriesLength = xV.length;
			for (int k = 0; k < seriesLength; k++) {
				series.add(xV[k], yV[k]);
			}
			dataset.addSeries(series);
		}
	}

	private XYMultipleSeriesRenderer buildRenderer(int[] colors,
			PointStyle[] styles) {
		XYMultipleSeriesRenderer renderer = new XYMultipleSeriesRenderer();
		setRenderer(renderer, colors, styles);
		return renderer;
	}

	private void setRenderer(XYMultipleSeriesRenderer renderer, int[] colors,
			PointStyle[] styles) {
		renderer.setAxisTitleTextSize(16);
		renderer.setChartTitleTextSize(20);
		renderer.setLabelsTextSize(15);
		renderer.setLegendTextSize(15);
		renderer.setPointSize(5f);
		renderer.setMargins(new int[] { 20, 30, 15, 20 });
		int length = colors.length;
		for (int i = 0; i < length; i++) {
			XYSeriesRenderer r = new XYSeriesRenderer();
			r.setColor(colors[i]);
			r.setPointStyle(styles[i]);
			renderer.addSeriesRenderer(r);
		}
	}

	private void setChartSettings(XYMultipleSeriesRenderer renderer,
			String title, String xTitle, String yTitle, double xMin,
			double xMax, double yMin, double yMax, int axesColor,
			int labelsColor) {
		renderer.setChartTitle(title);
		renderer.setChartTitleTextSize(90);
		renderer.setLabelsTextSize(30);
		renderer.setXTitle(xTitle);
		renderer.setYTitle(yTitle);
		renderer.setXAxisMin(xMin);
		renderer.setXAxisMax(xMax);
		renderer.setYAxisMin(yMin);
		renderer.setYAxisMax(yMax);
		renderer.setAxesColor(axesColor);
		renderer.setLabelsColor(labelsColor);
	}	
	
	
}
