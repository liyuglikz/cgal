
#ifndef QT_WIDGET_CIRCLE_SEGMENT_2_H
#define QT_WIDGET_CIRCLE_SEGMENT_2_H

#include <CGAL/IO/Qt_widget.h>
#include <CGAL/Arr_traits_2/Circle_segment_2.h>
#include <CGAL/Simple_cartesian.h>
#include <cmath>

namespace CGAL
{
  template < class Kernel, bool Filter >
CGAL::Qt_widget &
operator<<(CGAL::Qt_widget & widget,
           const CGAL::_Circle_segment_2<Kernel, Filter> &arc)
{
  if(arc.orientation() == COLLINEAR)
  {
    typedef Simple_cartesian<double> DK;
    typedef DK::Segment_2            DS;
    typedef DK::Point_2              DP;
    double sx = CGAL::to_double(arc.source().x());
    double sy = CGAL::to_double(arc.source().y());
    double tx = CGAL::to_double(arc.target().x());
    double ty = CGAL::to_double(arc.target().y());
    DS seg(DP(sx ,sy), DP(tx, ty));
    widget << seg;
    return (widget);
  }

  
  const typename Kernel::Circle_2 & circ = arc.supporting_circle();
  const typename Kernel::Point_2 & center = circ.center();
  typedef typename _X_monotone_circle_segment_2<Kernel, Filter>::Point_2 Arc_point_2;
  Arc_point_2 source;
  Arc_point_2 target;
  if(arc.orientation() == COUNTERCLOCKWISE)
  {
    source = arc.source();
    target = arc.target();
  }
  else
  {
    target = arc.source();
    source = arc.target();
  }
  double rad = std::sqrt(CGAL::to_double(circ.squared_radius()));

  int x_screen   = widget.x_pixel(CGAL::to_double(center.x()));
  int y_screen   = widget.y_pixel(CGAL::to_double(center.y()));
  int x_screen_b = widget.x_pixel(CGAL::to_double(center.x()) + rad);
  int radius     = x_screen_b - x_screen;

  double a   = std::atan2( to_double(source.y() - center.y()),
                            to_double(source.x() - center.x())); 
  double a2p = std::atan2( to_double(target.y() - center.y()),
                            to_double(target.x() - center.x()));

  if (a2p <= a)
      a2p += 2 * CGAL_PI;

  double alen2 = a2p - a;

  double diff = 180/CGAL_PI*16;

  widget.get_painter().drawArc(x_screen - radius, 
                                y_screen - radius, 
                                2 * radius, 2 * radius, 
                                (int)(a * diff), 
                                (int)(alen2 * diff));
  return widget;
}

}//end namespace CGAL
#endif
