#ifndef TOMTOM_NAVKIT2_MAPVIEW_HPP
#define TOMTOM_NAVKIT2_MAPVIEW_HPP

#include <memory>

#include <wx/event.h>
#include <wx/glcanvas.h>
#include <TomTom/NavKit/Map/Coordinate.hpp>
#include <TomTom/NavKit/Map/Map.hpp>
#include <TomTom/NavKit/Map/MapClickListener.hpp>
#include <TomTom/NavKit/Map/MapHolder.hpp>
#include <TomTom/NavKit/Map/SurfaceAdapter.hpp>

#include "tomtom/navkit2/mapdisplay/gesture_detector.hpp"
#include "tomtom/navkit2/mapdisplay/map_environment.hpp"

class MapView : public wxGLCanvas,
                  private TomTom::NavKit::Map::SurfaceAdapter::SurfaceAdapterDelegate {
 public:
  MapView(wxWindow* parent, const std::string& assets_path);
  virtual ~MapView();

  TomTom::NavKit::Map::MapHolder& MapHolder() {
    return *holder_;
  }

  std::shared_ptr<TomTom::NavKit::Map::Map> Map() {
    return MapHolder().getMap();
  }

 private:  // from SurfaceAdapterDelegate
  void requestDraw() override;

 private:
  // events
  void Render(wxPaintEvent& event);
  void Resized(wxSizeEvent& event);
  void MouseEvent(wxMouseEvent& event);
  void PinchGesture(wxZoomGestureEvent& event);
  void RotateGesture(wxRotateGestureEvent& event);

  DECLARE_EVENT_TABLE()

 private:
  mapdisplay::MapEnvironment environment_;
  std::unique_ptr<wxGLContext> context_;
  std::unique_ptr<TomTom::NavKit::Map::MapHolder> holder_;
  TomTom::NavKit::Map::Layer& marker_layer_;
  mapdisplay::GestureDetector gestures_;
  bool initialized_{false};
};

#endif  // TOMTOM_NAVKIT2_EXAMPLEAPP_MAPVIEW_HPP
