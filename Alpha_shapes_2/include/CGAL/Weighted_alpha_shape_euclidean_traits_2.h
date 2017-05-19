// Copyright (c) 1997  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
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
//
// Author(s)     : Tran Kai Frank DA <Frank.Da@sophia.inria.fr>
//                 Andreas Fabri <Andreas.Fabri@geometryfactory.com>

#ifndef CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_2_H
#define CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_2_H

#include <CGAL/license/Alpha_shapes_2.h>

// temporarily silenced
/*
#define CGAL_DEPRECATED_HEADER "<CGAL/Weighted_alpha_shape_euclidean_traits_2.h>"
#define CGAL_REPLACEMENT_HEADER "<CGAL/Regular_triangulation_euclidean_traits_2.h>"
#define CGAL_DEPRECATED_MESSAGE_DETAILS \
  "The class Weighted_alpha_shape_euclidean_traits_2<K> is deprecated in favor of "\
  "Regular_triangulation_euclidean_traits_2<K>."
#include <CGAL/internal/deprecation_warning.h>
*/

#include <CGAL/Regular_triangulation_euclidean_traits_2.h>

namespace CGAL {

template< class R >
class Weighted_alpha_shape_euclidean_traits_2 : public
  Regular_triangulation_euclidean_traits_2<R, typename R::FT>
{};

} //namespace CGAL


#endif //CGAL_WEIGHTED_ALPHA_SHAPE_EUCLIDEAN_TRAITS_2_H
