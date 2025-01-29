#include "MonitorWidget.h"
#include <math.h>

MonitorWidget::MonitorWidget(ImVec4 graphpos, ImVec4 infopos, ImVec4 color, std::string name, double top, double bottom, int lineWidth, int deadzone, double lengthInSeconds)
{
	_graphPosition = graphpos;
	_infoPosition = infopos;
	_color = color;
	_name = name;
	_top = top;
	_bottom = bottom;
	_lineWidth = lineWidth;
	_deadzoneSize = deadzone; // Size of the bar going through the pixels
	_lengthInSeconds = lengthInSeconds; // Length of the graph in seconds for conversion to pixels


	_currentChangePosition = 0;

	if ( _graphPosition.w - _deadzoneSize > 0 ) {
		_values = std::vector<double>(_graphPosition.z - _deadzoneSize);
		_updateValues = std::vector<double>(_graphPosition.z - _deadzoneSize);

		std::fill(_values.begin(), _values.end(), 0.5);
		std::fill(_updateValues.begin(), _updateValues.end(), 0.5);
		std::cout << "Length: " << _graphPosition.z << std::endl;
	}
}
MonitorWidget::MonitorWidget() {
}
MonitorWidget::~MonitorWidget() {
	_values.resize(0);
	_updateValues.resize(0);
}

double calculateValue(uint64_t i, uint64_t div) {
	// if(div == 0) std::cout << "Divider is zero!" << std::endl;
	const int divider = (div + 1);
	return ((double)(i % divider) / (double)divider);
}
bool MonitorWidget::needsUpdate() {
	if(_needsUpdate) {
		_needsUpdate = false;
		return true;
	}
	return false;
}
void MonitorWidget::changeValue(int pixelOffset, double value, bool interpolate) {
	// std::cout << "start" << std::endl;
	if(interpolate) {
		double start = _values[_lastUpdatedValuePos];
		int totalPixels = pixelOffset - _lastUpdatedValuePos;
		for(int i = 0; i <= totalPixels; i++) {
			// start = 0% and value is 100%
			// _lastUpdatedValuePos = 0% and pixelOffset = 100% -> i =
			double mp = (double)(i) / (double)(totalPixels);
			double val = start + mp * (value - start);
			if(val != NAN) {
				_values[_lastUpdatedValuePos + i] = val;
			} else {
				_values[_lastUpdatedValuePos + i] = 0.5 * _top;
			}
			// std::cout << "Calculated " << _lastUpdatedValuePos + i << " to " << pixelOffset << " with mp " << val << std::endl;
		}
	} else {
		_values[pixelOffset] = value;
	}
	_lastUpdatedValuePos = pixelOffset;
}
void MonitorWidget::setValue(int t, double v) {
	if(t < _updateValues.size()) {
		_updateValues.at(t) = v;
	}
}

void MonitorWidget::render(time_t t, time_t dt) {
    if (_values.empty()) return;

    // Calculate graph width in pixels and handle time offset
    int graphWidthInPixels = static_cast<int>(_graphPosition.z); // Assuming z is the width
    if (graphWidthInPixels <= 0) return;


    double timeOffset = static_cast<double>(t % static_cast<time_t>(_lengthInSeconds));
    double pixelOffset = (timeOffset / _lengthInSeconds) * graphWidthInPixels;

    // Wrap pixelOffset within graph bounds
    pixelOffset = fmod(pixelOffset, graphWidthInPixels);

    // Deadzone bar position
    float deadzoneStart = static_cast<float>(_graphPosition.x + pixelOffset);
    float deadzoneEnd = static_cast<float>(deadzoneStart + _deadzoneSize);

    // Update values in the deadzone
    int startIndex = static_cast<int>((pixelOffset / graphWidthInPixels) * _values.size());
    int endIndex = static_cast<int>((_currentChangePosition / graphWidthInPixels) * _values.size());



    #ifdef RENDER_DEBUG
    RenderingEngine::debugBound(_graphPosition);
	RenderingEngine::debugBound(_infoPosition);
	#endif

    // Render the graph using OpenGL
    glPushAttrib(GL_ENABLE_BIT); // Save OpenGL state
    glDisable(GL_TEXTURE_2D);   // Disable texture rendering for solid colors
    glLineWidth(_lineWidth);    // Set line width

    // Draw the graph lines
    glBegin(GL_LINES);
    glColor3f(_color.x, _color.y, _color.z); // Set color to white

	size_t newestDrawnPoint = floor(((deadzoneStart + deadzoneEnd) / 2 - _graphPosition.x) / graphWidthInPixels * _values.size());
	size_t calculatedDrawnAmount = newestDrawnPoint - _lastDrawnValuePos;

	if(newestDrawnPoint < _lastDrawnValuePos) {
		_needsUpdate = true;
	}

	// UNCOMMENT FOR RANDOM DATA
	// if(newestDrawnPoint < _lastDrawnValuePos) {
	// 	calculatedDrawnAmount = newestDrawnPoint + (_values.size() - _lastDrawnValuePos);
	// 	// std::cout << calculatedDrawnAmount << std::endl;
	// 	int rndVal = rand() % 1000;
	// 	int rndBase = rand() % _updateValues.size();
	// 	_recentRand = rndBase;
	// 	// int rndBase = 4294967295;
	// 	for (int i = 0; i < _updateValues.size(); i++) {
	// 		double v = calculateValue(rndBase + i, rndVal);
	// 		// std :: cout << "Calculated " << v << std::endl;
	// 		_updateValues[i] = v;
	// 	}
	// }
	for (size_t i = 0; i < calculatedDrawnAmount; i++) {
		size_t pos = (_lastDrawnValuePos + i) % _values.size();
		_values[pos] = _updateValues[pos];
		// std::cout << _values[pos] << std::endl;
	}
	// _lastDrawnValuePos = newestDrawnPoint;

    for (size_t i = 1; i < _values.size(); ++i) {
        // Map points to screen coordinates
        float x1 = _graphPosition.x + static_cast<float>((i - 1) * graphWidthInPixels / _values.size());
        float x2 = _graphPosition.x + static_cast<float>(i * graphWidthInPixels / _values.size());

        // Wrap x coordinates within bounds
        if (x1 < _graphPosition.x) x1 += graphWidthInPixels;
        if (x2 < _graphPosition.x) x2 += graphWidthInPixels;

        // Map Y positions to graph height
        float y1 = _graphPosition.y + static_cast<float>(_graphPosition.w * (1.0 - _values[i - 1]));
        float y2 = _graphPosition.y + static_cast<float>(_graphPosition.w * (1.0 - _values[i]));
		// std::cout << "1:\t(" << x1 << ",\t" << y1 << ")" << std::endl;
		// std::cout << "2:\t(" << x2 << ",\t" << y2 << ")" << std::endl;
		// std::cout << _values[i - 1];

        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    }
    glEnd();

	// _values[floor(((deadzoneStart + deadzoneEnd) / 2 - _graphPosition.x) / graphWidthInPixels * _values.size())] = calculateValue(t, 700);
	// changeValue(floor(((deadzoneStart + deadzoneEnd) / 2 - _graphPosition.x) / graphWidthInPixels * _values.size()), calculateValue(t, 800), true);

    // Draw the deadzone bar
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f); // Semi-transparent red
    glBegin(GL_QUADS);
    glVertex2f(deadzoneStart, _graphPosition.y);
    glVertex2f(deadzoneEnd, _graphPosition.y);
    glVertex2f(deadzoneEnd, _graphPosition.y + _graphPosition.w);
    glVertex2f(deadzoneStart, _graphPosition.y + _graphPosition.w);
    glEnd();
    glPopAttrib(); // Restore OpenGL state


	// Imgui part
	ImGuiWindowFlags wf = 0;
	wf |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
	// wf |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	_open = true;

	ImGui::SetNextWindowPos(ImVec2(_infoPosition.x, _infoPosition.y));

	ImGui::Begin(_name.c_str(), &_open, wf);
	ImGui::SetWindowSize(ImVec2(_infoPosition.z, _infoPosition.w));
	// ImGui::Text("%s", _name.c_str());
	// ImGui::SetWindowFontScale(4.0f);
	// float oldSize = ImGui::GetFont()->Scale;
	// ImGui::GetFont()->Scale *= 4;
	// ImGui::PushFont(ImGui::GetFont());
	// ImGui::Text("%s: %i", _name.c_str(), _recentRand);
	ImGui::Text("%s: %i", _name.c_str(), 500);
	// ImGui::SetWindowFontScale(1.0f);
	// ImGui::GetFont()->Scale = oldSize;
	// ImGui::PopFont();
	ImGui::End();


	// RenderingEngine::renderText(_name.c_str(), {.r = 255, .g = 255, .b = 255, .a = 255 }, _infoPosition.x, _infoPosition.y, RenderingEngine::_basefont);
}

