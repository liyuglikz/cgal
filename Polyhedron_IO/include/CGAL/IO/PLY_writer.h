// Copyright (c) 2017 GeometryFactory
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Simon Giraudot

#ifndef CGAL_IO_PLY_WRITER_H
#define CGAL_IO_PLY_WRITER_H

#include <CGAL/license/Polyhedron.h>

#include <CGAL/IO/write_ply_points.h>

namespace CGAL{

  template <class Point_3, class Polygon_3>
  bool
  write_PLY(std::ostream& out,
            std::vector< Point_3 >& points,
            std::vector< Polygon_3 >& polygons,
            bool /* verbose */ = false)
  {

    if(!out)
    {
      std::cerr << "Error: cannot open file" << std::endl;
      return false;
    }

    // Write header
    out << "ply" << std::endl
        << ((get_mode(out) == IO::BINARY) ? "format binary_little_endian 1.0" : "format ascii 1.0") << std::endl
        << "comment Generated by the CGAL library" << std::endl
        << "element vertex " << points.size() << std::endl;
    
    internal::PLY::output_property_header (out,
                                           make_ply_point_writer (CGAL::Identity_property_map<Point_3>()));

    out << "element face " << polygons.size() << std::endl;
  
    internal::PLY::output_property_header (out,
                                           std::make_pair (CGAL::Identity_property_map<Polygon_3>(),
                                                           PLY_property<std::vector<int> >("vertex_index")));
    
    out << "end_header" << std::endl;
  
    for (std::size_t i = 0; i < points.size(); ++ i)
      internal::PLY::output_properties (out, points.begin() + i,
                                        make_ply_point_writer (CGAL::Identity_property_map<Point_3>()));

    for (std::size_t i = 0; i < polygons.size(); ++ i)
      internal::PLY::output_properties (out, polygons.begin() + i,
                                        std::make_pair (CGAL::Identity_property_map<Polygon_3>(),
                                                        PLY_property<std::vector<int> >("vertex_index")));

    return out.good();
  }


} // namespace CGAL

#endif // CGAL_IO_PLY_WRITER_H
