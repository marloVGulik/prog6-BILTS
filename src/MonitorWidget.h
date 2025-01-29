#ifndef MONITOR_WIDGET
#define MONITOR_WIDGET

#include <vector>
#include <time.h>
#include <string>
#include "RenderingEngine.h"

class MonitorWidget {
public:
	MonitorWidget();
	MonitorWidget(ImVec4 graphpos, ImVec4 infopos, ImVec4 color, std::string name, double top, double bottom, int lineWidth, int deadzone, double lengthInSeconds);
	~MonitorWidget();

	void changeGraphWidth(int width);

	void render(time_t t, time_t dt);

	void changeValue(int timeSet, double value, bool interpolate);
	void setValue(int t, double v);

	bool needsUpdate();
private:
	// Base values
	ImVec4 _graphPosition;
	ImVec4 _infoPosition;
	ImVec4 _color;
	std::string _name;
	double _top;
	double _bottom;
	int _lineWidth;
	int _deadzoneSize; // Size of the bar going through the pixels
	double _lengthInSeconds; // Length of the graph in seconds for conversion to pixels

	// Frequently updating values
	int _currentChangePosition;
	std::vector<double> _updateValues;
	std::vector<double> _values; // Queue length is graph width in pixels

	// ChangeValue
	int _lastDrawnValuePos = 0;
	int _lastUpdatedValuePos = 0;

	// Imgui
	bool _open;
	int _recentRand;

	bool _needsUpdate;
};

#endif
