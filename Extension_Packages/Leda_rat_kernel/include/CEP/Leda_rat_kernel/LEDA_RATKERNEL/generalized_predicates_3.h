#ifndef CEP_LEDA_RAT_GENERALIZED_PREDICATES_3_H
#define CEP_LEDA_RAT_GENERALIZED_PREDICATES_3_H

#include <CGAL/basic.h>

#include <CEP/Leda_rat_kernel/LEDA_RATKERNEL/d3_rat_support_functions.h>

// LEDA rational kernel generalized predicates ...

// 3d generalized predicates  ...

#if (__LEDA__ == 420)
// fix a linker problem (undefined reference)
leda_rat_vector::leda_rat_vector(leda_rational a, leda_rational b, leda_rational c)
{ 
  leda_integer a_num = a.numerator();
  leda_integer b_num = b.numerator();
  leda_integer c_num = c.numerator();
  leda_integer a_den = a.denominator();
  leda_integer b_den = b.denominator();
  leda_integer c_den = c.denominator();

  PTR = new geo_rep(a_num*b_den*c_den, b_num*a_den*c_den, c_num*a_den*b_den,
                    a_den*b_den*c_den);
}
#endif

CGAL_BEGIN_NAMESPACE

template<class K>
class Predicate_leda_d3_rat_angle {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Vector_3     Vector_3;
  typedef typename K::FT           FT;

public:
  typedef Arity_tag< 3 > Arity;
  typedef CGAL::Angle    result_type;
  
  // we just return the sign of the dot product ...
  
  CGAL::Angle operator()(const Point_3& p1,const Point_3& p2,
                         const Point_3& p3)
  {
    // get vectors from p2-p1 and from p2-p3
    Vector_3 v1 = p1-p2;
    Vector_3 v2 = p3-p2;
  
    FT s_prod = v1*v2;
  
    if (s_prod == 0) return CGAL::RIGHT;
    if (s_prod >  0) return CGAL::ACUTE;
    return CGAL::OBTUSE;
  }
};

template<class K>
class Predicate_leda_d3_rat_equal {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Vector_3     Vector_3;
  typedef typename K::Line_3       Line_3;
  typedef typename K::Direction_3  Direction_3;
  typedef typename K::Plane_3      Plane_3; 
  typedef typename K::Ray_3        Ray_3;
  typedef typename K::Triangle_3   Triangle_3;
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Segment_3    Segment_3;     
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;      
  typedef typename K::FT           FT;

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

  bool operator()(const Point_3& p1, const Point_3& p2) const
  {
    return ( p1 == p2 );
  }

  bool operator()(const Vector_3& v1, const Vector_3& v2) const
  {
    return ( v1 == v2 );
  }
  
  bool operator()(const Direction_3& d1, const Direction_3& d2) const
  {
    Vector_3 v1 = d1.get_vector();
    Vector_3 v2 = d2.get_vector();
    
    CGAL_precondition( (v1.dim()==3) && (v2.dim()==3));
    
    FT xc1 = v1[0];
    FT xc2 = v2[0];
    FT yc1 = v1[1];
    FT yc2 = v2[1];
    FT zc1 = v1[2];
    FT zc2 = v2[2];    
    
    FT q1,q2,q3;
    
    // equal directions ???    
    if (xc2==0) {
      if (xc1!=0) return false;
      else q1 = 0;
    }
    else q1 = xc1/xc2;
    
    if (yc2==0) {
      if (yc1!=0) return false;
      else q2 = 0;
    }
    else q2 = yc1/yc2;
    
    if (zc2==0) {
      if (zc1!=0) return false;
      else q3 = 0;
    }
    else q3 = zc1/zc2;     
    
    if (q1==q2 && q1==q3) return true;
    return false;
  }
  
  bool operator()(const Line_3& l1, const Line_3& l2) const
  {
#if (__LEDA__ <= 420)
    if (! ((Line_3&)l1 == (Line_3&)l2)) return false;
#else  
    if (! (l1 == l2)) return false;
#endif
    
    // same direction ???
    Vector_3 v1 = l1.to_vector();
    Vector_3 v2 = l2.to_vector();
    
    // compare directions ...
    Direction_3 d1(v1);
    Direction_3 d2(v2);
    return this->operator()(d1,d2);
  }    
  
  bool operator()(const Plane_3& p1, const Plane_3& p2) const
  {
    return (p1 == p2);
  }
  
  bool operator()(const Ray_3& r1, const Ray_3& r2) const
  {
    if (r1.point1() != r2.point1()) return false;
    
    // compare directions ...
    Vector_3 v1 = r1.to_vector();
    Vector_3 v2 = r2.to_vector();
    
    // compare directions ...
    Direction_3 d1(v1);
    Direction_3 d2(v2);
    return this->operator()(d1,d2);    
  }  

  bool operator()(const Segment_3& s1, const Segment_3& s2) const
  {
    return (s1 == s2);
  }  

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s1,
                  const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s2) const
  {
    return (s1 == s2);
  }
#else
  bool operator()(const leda_d3_rat_sphere& s1, const leda_d3_rat_sphere& s2) const
  {
    if (s1.is_degenerate() && s2.is_degenerate()) return true;
    Point_3 a1 = s1.point1();
    Point_3 a2 = s1.point2();
    Point_3 a3 = s1.point3();
    Point_3 a4 = s1.point4();
    
    if (s2.contains(a1) && s2.contains(a2) && s2.contains(a3) && s2.contains(a4)) return true;
    return false;
  }   
#endif

  bool operator()(const Triangle_3& t1, const Triangle_3& t2) const
  {
#if (__LEDA__ <= 420)
    return ((Triangle_3&)t1 == (Triangle_3&)t2);
#else  
    return (t1 == t2);
#endif    
  }  
  
  bool operator()(const Tetrahedron_3& s1,
                  const Tetrahedron_3& s2) const
  {
    return (s1 == s2);
  }   
  
  bool operator()(const Iso_cuboid_3& ic1,
                  const Iso_cuboid_3& ic2) const
  {
     Point_3 lower_left1 = ic1.vertex(0);
     Point_3 lower_left2 = ic2.vertex(0);    
     Point_3 upper_right1 = ic1.vertex(7);
     Point_3 upper_right2 = ic2.vertex(7);  
     
     if (lower_left1==lower_left2 && upper_right1==upper_right2) return true;
     return false;       
  }     
   
};

template<class K>
class Predicate_leda_d3_rat_equal_x {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3      __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (LEDA_NAMESPACE_NAME::identical(p1,p2)) return true;
     if (__My_Point_3::cmp_x(p1,p2) == 0) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_equal_y {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3      __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (LEDA_NAMESPACE_NAME::identical(p1,p2)) return true;   
     if (__My_Point_3::cmp_y(p1,p2) == 0) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_equal_z {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3      __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (LEDA_NAMESPACE_NAME::identical(p1,p2)) return true;   
     if (__My_Point_3::cmp_z(p1,p2) == 0) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_equal_xy {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3      __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (LEDA_NAMESPACE_NAME::identical(p1,p2)) return true;   
     if (__My_Point_3::cmp_x(p1,p2) == 0 && __My_Point_3::cmp_y(p1,p2) == 0) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_equal_xyz {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3      __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (LEDA_NAMESPACE_NAME::identical(p1,p2)) return true;   
     if (__My_Point_3::cmp_xyz(p1,p2) == 0) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_less_x {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (__My_Point_3::cmp_x(p1,p2) == -1) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_less_y {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (__My_Point_3::cmp_y(p1,p2) == -1) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_less_z {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (__My_Point_3::cmp_z(p1,p2) == -1) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_less_xy {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     int cmx = __My_Point_3::cmp_x(p1,p2);
     
     if (cmx == -1) return true;
     else {
      if (cmx==0 && __My_Point_3::cmp_y(p1,p2) == -1) return true;
     }
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_less_xyz {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2) const
   {
     if (__My_Point_3::cmp_xyz(p1,p2) == -1) return true;
     return false;   
   }
};

template<class K>
class Predicate_leda_d3_rat_compare_x {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2) const
  {
     return ( (Comparison_result) __My_Point_3::cmp_x(p1,p2));     
  }
};

template<class K>
class Predicate_leda_d3_rat_compare_y {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2) const
  {
     return ( (Comparison_result) __My_Point_3::cmp_y(p1,p2));     
  }
};

template<class K>
class Predicate_leda_d3_rat_compare_z {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2) const
  {
     return ( (Comparison_result) __My_Point_3::cmp_z(p1,p2));     
  }
};

template<class K>
class Predicate_leda_d3_rat_compare_xy {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2) const
  {
     int cmx = __My_Point_3::cmp_x(p1,p2);    
     if (cmx != 0) return  ((Comparison_result) cmx);
     int cmy = __My_Point_3::cmp_y(p1,p2);
          
     return ( (Comparison_result) cmy);     
  }
};

template<class K>
class Predicate_leda_d3_rat_compare_xyz {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2) const
  {
     return ( (Comparison_result) __My_Point_3::cmp_xyz(p1,p2));     
  }
};


// filter this later ...

template<class K>
class Predicate_leda_d3_rat_less_signed_distance_to_plane {

  typedef typename K::Point_3      Point_3; 
  typedef typename K::Plane_3      Plane_3; 
  typedef typename K::FT           FT;  

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

  bool operator()(const Plane_3& pl,
                  const Point_3& p1, 
	          const Point_3& p2) const
  {
     int ori1 = pl.side_of(p1);
     int ori2 = pl.side_of(p2);
  
     FT d1 = (pl.sqr_dist(p1))*ori1;
     FT d2 = (pl.sqr_dist(p2))*ori2;     
     return (d1 < d2);
  }   
};


template<class K>
class Predicate_leda_d3_rat_less_distance_to_point {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2,
                   const Point_3& p3) const
   {
     int res = LEDA_NAMESPACE_NAME::cmp_distances(p1,p2,p1,p3);
     
     if (res==-1) return true;
     return false;
   }
};

template<class K>
class Predicate_leda_d3_rat_compare_distance {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 3 > Arity;
  typedef Comparison_result       result_type;

  Comparison_result operator()(const Point_3& p1, const Point_3& p2,
                               const Point_3& p3) const
  {
     return ( (Comparison_result) LEDA_NAMESPACE_NAME::cmp_distances(p1,p2,p1,p3));     
  }  
};

template<class K>
class Predicate_leda_d3_rat_collinear {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2,
                   const Point_3& p3) const
   {
      return LEDA_NAMESPACE_NAME::collinear(p1,p2,p3);
   }
};

template<class K>
class Predicate_leda_d3_rat_coplanar {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 4 > Arity;
  typedef bool       result_type;

   bool operator()(const Point_3& p1, const Point_3& p2,
                   const Point_3& p3, const Point_3& p4) const
   {
      return LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p4);   
   }
};

template<class K>
class Predicate_leda_d3_rat_orientation {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 4 > Arity;
  typedef Orientation       result_type;

  Orientation operator()(const Point_3& p1, const Point_3& p2, 
                         const Point_3& p3, const Point_3& p4) const
  {
     return (Orientation) LEDA_NAMESPACE_NAME::orientation(p1,p2,p3,p4);
  }
};

template<class K>
class Predicate_leda_d3_rat_coplanar_orientation {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Orientation       result_type;
  typedef Arity_tag< 3 >          Arity;

  Orientation operator()(const Point_3& p1, const Point_3& p2, 
                         const Point_3& p3) const
  {
     if (LEDA_NAMESPACE_NAME::collinear(p1,p2,p3)) return CGAL::COLLINEAR;
     
     // return 2d orientation ...
     // find projection plane :
     Orientation ori_xy = (Orientation) (LEDA_NAMESPACE_NAME::orientation_xy(p1,p2,p3));
     if ( ori_xy != CGAL::COLLINEAR ) return ori_xy;
     
     Orientation ori_xz = (Orientation) (LEDA_NAMESPACE_NAME::orientation_xz(p1,p2,p3));
     if ( ori_xz != CGAL::COLLINEAR ) return ori_xz;
     
     return (Orientation) (LEDA_NAMESPACE_NAME::orientation_yz(p1,p2,p3));       
  }

  Orientation operator()(const Point_3& p1, const Point_3& p2, 
                         const Point_3& p3, const Point_3& p4) const
  {
     // Preconditions:
     CGAL_precondition(! LEDA_NAMESPACE_NAME::collinear(p1,p2,p3));
     CGAL_precondition(LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p4));
     
     // find a plane; project and compute orientation ...
     Orientation ori_xy = (Orientation) (LEDA_NAMESPACE_NAME::orientation_xy(p1,p2,p3));

    if ( ori_xy != CGAL::COLLINEAR )
      // the projection in (x,y) - plane is OK
      return Orientation( ori_xy * LEDA_NAMESPACE_NAME::orientation_xy(p1,p2,p4) );

    // we have to project onto another plane :

    if ((__My_Point_3::cmp_x(p1,p2) != 0) || (__My_Point_3::cmp_x(p1,p3) != 0))
    {
      // projection into (x,z)-plane is ok
      return Orientation ( LEDA_NAMESPACE_NAME::orientation_xz(p1,p2,p3) * 
                           LEDA_NAMESPACE_NAME::orientation_xz(p1,p2,p4) ); 
    }
    
    // projection into (y,z)-plane
    return Orientation ( LEDA_NAMESPACE_NAME::orientation_yz(p1,p2,p3) * 
                         LEDA_NAMESPACE_NAME::orientation_yz(p1,p2,p4) );
         
  }
};

template<class K>
class Predicate_leda_d3_rat_coplanar_side_of_bounded_circle {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 4 > Arity;
  typedef Bounded_side       result_type;

  Bounded_side operator()(const Point_3& p1, const Point_3& p2, 
                          const Point_3& p3, const Point_3& test) const
  {
    // Preconditions:
    CGAL_precondition(! LEDA_NAMESPACE_NAME::collinear(p1,p2,p3));
    CGAL_precondition(LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,test));

    Point_3 t1(0,0,0), t2(1,0,0), t3(0,1,0), t4(0,0,1);     
    
    // find a 4. point for the sphere ...
    int ori;
    Point_3 p4 = ((ori = LEDA_NAMESPACE_NAME::orientation(p1,p2,p3,t1)) != 0) ? t1:
              ((ori = LEDA_NAMESPACE_NAME::orientation(p1,p2,p3,t2)) != 0) ? t2:
              ((ori = LEDA_NAMESPACE_NAME::orientation(p1,p2,p3,t3)) != 0) ? t3: 
              ((ori = LEDA_NAMESPACE_NAME::orientation(p1,p2,p3,t4)) != 0) ? t4: t4;
    
    // now region of sphere test:
    int res = (ori * LEDA_NAMESPACE_NAME::side_of_sphere(p1,p2,p3,p4,test));  
    if (res==0) return CGAL::ON_BOUNDARY;
    if (res==-1) return CGAL::ON_UNBOUNDED_SIDE;
    return CGAL::ON_BOUNDED_SIDE;        
  }
};

template<class K>
class Predicate_leda_d3_rat_side_of_oriented_sphere {

  typedef typename K::Point_3      Point_3;

public:
  typedef Arity_tag< 5 >  Arity;
  typedef Oriented_side   result_type;

  Oriented_side operator()(const Point_3& p1, const Point_3& p2, 
                           const Point_3& p3, const Point_3& p4,
			   const Point_3& test) const
  {
     return (Oriented_side) LEDA_NAMESPACE_NAME::side_of_sphere(p1,p2,p3,p4,test);
  }
};

template<class K>
class Predicate_leda_d3_rat_side_of_bounded_sphere {

  typedef typename K::Point_3      Point_3;

public:
  typedef Bounded_side       result_type;
  typedef Arity_tag< 5 >     Arity;

  Bounded_side operator()(const Point_3& p1, const Point_3& p2, 
                          const Point_3& p3, const Point_3& p4,
			  const Point_3& test) const
  {
     int res = LEDA_NAMESPACE_NAME::region_of_sphere(p1,p2,p3,p4,test);
     if (res==0) return CGAL::ON_BOUNDARY;
     if (res==-1) return CGAL::ON_UNBOUNDED_SIDE;
     return CGAL::ON_BOUNDED_SIDE;     
  }

  // center of sphere must be in plane through p1,p2,p3 ...
  Bounded_side operator()(const Point_3& p1, const Point_3& p2, 
                          const Point_3& p3, const Point_3& test) const
  {
     Point_3 m = leda_support::construct_circle_center_3(p1,p2,p3);
     int res = LEDA_NAMESPACE_NAME::cmp_distances(m,p1,m,test);
     if (res==0) return CGAL::ON_BOUNDARY;
     if (res==-1) return CGAL::ON_UNBOUNDED_SIDE;
     return CGAL::ON_BOUNDED_SIDE;     
  }
  
  Bounded_side operator()(const Point_3& p1, const Point_3& p2, 
                          const Point_3& test) const
  {
     // compute midpoint of sphere with diameter p1p2; then compare distances
     // we could filter this later ...
     Point_3 m = LEDA_NAMESPACE_NAME::midpoint(p1,p2);
     int res = LEDA_NAMESPACE_NAME::cmp_distances(m,p1,m,test);
     if (res==0) return CGAL::ON_BOUNDARY;
     if (res==-1) return CGAL::ON_UNBOUNDED_SIDE;
     return CGAL::ON_BOUNDED_SIDE;
  } 

};

template<class K>
class Predicate_leda_d3_rat_is_degenerate {

  typedef typename K::Line_3       Line_3;
  typedef typename K::Plane_3      Plane_3; 
  typedef typename K::Ray_3        Ray_3;
  typedef typename K::Triangle_3   Triangle_3;
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Segment_3    Segment_3;     
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;
  typedef typename K::Vector_3     Vector_3;  

public:
  typedef Arity_tag< 1 > Arity;
  typedef bool       result_type;

  bool operator()(const Iso_cuboid_3& c) const
  {
    // are the vertices collinear ???
    return LEDA_NAMESPACE_NAME::collinear(c.vertex(0),c.vertex(5),c.vertex(7));
  }
  
  bool operator()(const Line_3& l) const
  {
    return (l.point1() == l.point2());
  }
  
  bool operator()(const Plane_3& pl) const
  {
#if (__LEDA__ >= 440)      
    return ((pl.A() == 0) && (pl.B() == 0) && (pl.C() == 0));
#else
    Vector_3 n = pl.normal();
    return ((n.X() == 0) && (n.Y() == 0) && (n.Z() == 0));
#endif    
  }
  
  bool operator()(const Ray_3& r) const
  {
    return (r.point1() == r.point2());
  }    
  
  bool operator()(const Segment_3& seg) const
  {
    return (seg.source() == seg.target());
  }

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& sp) const
  {
    return (sp.squared_radius() == 0);
  }
#else  
  bool operator()(const leda_d3_rat_sphere& sp) const
  {
    return (sp.is_degenerate());
  }
#endif  
  
  bool operator()(const Tetrahedron_3& s) const
  {
    return s.is_degenerate();
  }
  
  bool operator()(const Triangle_3& t) const
  {
    return t.is_degenerate();  
  }            
};

template<class K>
class Predicate_leda_d3_rat_has_on {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Line_3       Line_3;
  typedef typename K::Plane_3      Plane_3; 
  typedef typename K::Ray_3        Ray_3;
  typedef typename K::Triangle_3   Triangle_3;
  typedef typename K::Segment_3    Segment_3;     

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Line_3& l, const Point_3& p) const
   {
     return l.contains(p);
   }
   
   bool operator()(const Ray_3& r, const Point_3& p) const
   {
     return r.contains(p);
   }   
   
   bool operator()(const Segment_3& s, const Point_3& p) const
   {
     return s.contains(p);  
   }     
   
   bool operator()(const Plane_3& pl, const Point_3& p) const
   {
     return pl.contains(p);  
   }     

   bool operator()(const Triangle_3& t, const Point_3& p) const
   {
     // coplanar ?
     Point_3 p1 = t.point1();
     Point_3 p2 = t.point2();
     Point_3 p3 = t.point3();
      
     if (! LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p)) return false;
     
     // triangle and point are coplanar ...
     if (t.is_degenerate()){
        if (p1==p2 && p2==p3){ // point
	  if (p==p1) return true;
	  return false;
	}
	// segment
	Segment_3 seg(p1,p2);
	if (! seg.contains(p3)){  // we need p3 ...
	  seg = Segment_3(p1,p3);
	  if (! seg.contains(p2)) seg = Segment_3(p2,p3);
	}
	
	return seg.contains(p);
     }
     // triangle is not degenerate; that means that one of the following projections
     // is a (non-degenerate) triangle 
     leda_rat_triangle tp = leda_support::project_xy(t); 
     leda_rat_point    pp = p.project_xy();
     if (! tp.is_degenerate()){
        LEDA_NAMESPACE_NAME::region_kind rk = tp.region_of(pp);
	if (rk == LEDA_NAMESPACE_NAME::UNBOUNDED_REGION) return false;
	return true;
     }
     
     tp = leda_support::project_yz(t);
     pp = p.project_yz();
     
     if (! tp.is_degenerate()){
        LEDA_NAMESPACE_NAME::region_kind rk = tp.region_of(pp);
	if (rk == LEDA_NAMESPACE_NAME::UNBOUNDED_REGION) return false;
	return true;
     }
     
     tp = leda_support::project_xz(t);
     pp = p.project_xz();
     LEDA_NAMESPACE_NAME::region_kind rk = tp.region_of(pp);
     if (rk == LEDA_NAMESPACE_NAME::UNBOUNDED_REGION) return false;
     return true;               
   }   
};

template<class K>
class Predicate_leda_d3_rat_has_on_bounded_side {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;  
  typedef typename K::FT           FT;    

  typedef typename K::Point_3   __My_Point_3;

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     if (d < sq) return true;
     return false;
      
  }
#else
   bool operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     return s.inside(p);
   }
#endif   

   bool operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
     // inside returns true if p is inside or on s ...
     
     bool inside = s.in_simplex(p);
     if (! inside) return false;
     
     // is the point ON the simplex ???
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();   
     
     if (LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p) ||  LEDA_NAMESPACE_NAME::coplanar(p1,p2,p4,p) ||
         LEDA_NAMESPACE_NAME::coplanar(p2,p3,p4,p) ||  LEDA_NAMESPACE_NAME::coplanar(p3,p1,p4,p)) return false; 
     
     return true;
   }   
   
   bool operator()(const Iso_cuboid_3& ic, const Point_3& p) const
   {
     Point_3 min = ic.vertex(0);
     Point_3 max = ic.vertex(7);
     
     // compare with x/y/z coordinates ...
     if (__My_Point_3::cmp_x(p,min) != 1 || __My_Point_3::cmp_x(p,max) != -1) return false;
     if (__My_Point_3::cmp_y(p,min) != 1 || __My_Point_3::cmp_y(p,max) != -1) return false;
     if (__My_Point_3::cmp_z(p,min) != 1 || __My_Point_3::cmp_z(p,max) != -1) return false;     
          
     return true;     
   }      
};

template<class K>
class Predicate_leda_d3_rat_has_on_unbounded_side {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;  
  typedef typename K::FT           FT;    
  
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     if (d > sq) return true;
     return false;
      
  }
#else
   bool operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     return s.outside(p);
   }
#endif

   bool operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
     bool inside = s.in_simplex(p);
     if (inside) return false;
     
     // is the point ON the simplex ???
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();   
     
     if (LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p) ||  LEDA_NAMESPACE_NAME::coplanar(p1,p2,p4,p) ||
         LEDA_NAMESPACE_NAME::coplanar(p2,p3,p4,p) ||  LEDA_NAMESPACE_NAME::coplanar(p3,p1,p4,p)) return false; 
     
     return true;
   }   
   
   bool operator()(const Iso_cuboid_3& ic, const Point_3& p) const
   {
     Point_3 min = ic.vertex(0);
     Point_3 max = ic.vertex(7);
     
     // compare with x/y/z coordinates ...
     if (__My_Point_3::cmp_x(p,min) != 1 || __My_Point_3::cmp_x(p,max) != -1) return true;
     if (__My_Point_3::cmp_y(p,min) != 1 || __My_Point_3::cmp_y(p,max) != -1) return true;
     if (__My_Point_3::cmp_z(p,min) != 1 || __My_Point_3::cmp_z(p,max) != -1) return true;     
          
     return false;     
   }      
};

template<class K>
class Predicate_leda_d3_rat_has_on_boundary {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Plane_3      Plane_3;
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;  
  typedef typename K::FT           FT;    
  
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;
  
  // undocumented
  bool operator()(const Plane_3& pl, const Point_3& p) const
  {
      return pl.contains(p);
  }  

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     if (d == sq) return true;
     return false;
      
  }
#else
   bool operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     return s.contains(p);
   }
#endif

   bool operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
     // s.inside returns true if p is in or on the simplex !!!!
     bool inside = s.in_simplex(p);
     if (! inside) return false;
     
     // is the point really ON the simplex ???
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();   
     
     if (LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p) ||  LEDA_NAMESPACE_NAME::coplanar(p1,p2,p4,p) ||
         LEDA_NAMESPACE_NAME::coplanar(p2,p3,p4,p) ||  LEDA_NAMESPACE_NAME::coplanar(p3,p1,p4,p)) return true; 
     
     return false;
   }   
   
   bool operator()(const Iso_cuboid_3& ic, const Point_3& p) const
   {
     Point_3 min = ic.vertex(0);
     Point_3 max = ic.vertex(7);
     
     // compare with x/y/z coordinates ...
     // we need at least one zero in the cmp results;
     int res1 = __My_Point_3::cmp_x(p,min);
     int res2 = __My_Point_3::cmp_x(p,max);
     
     if (res1 == -1 || res2 == 1) return false;
     
     int res3 = __My_Point_3::cmp_y(p,min);
     int res4 = __My_Point_3::cmp_y(p,max);     
     
     if (res3 == -1 || res4 == 1) return false;
     
     int res5 = __My_Point_3::cmp_z(p,min);
     int res6 = __My_Point_3::cmp_z(p,max);        
     
     if (res5 == -1 || res6 == 1) return false;     
          
     // we need at least one zero in the comparison results ...	  
     if (res1==0 || res2==0 || res3==0 || res4==0 || res5==0 || res6==0) return true;  	  
	  
     return false;     
   }      
};

template<class K>
class Predicate_leda_d3_rat_has_on_positive_side {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Plane_3      Plane_3;   
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::FT           FT;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool           result_type;

   bool operator()(const Plane_3& pl, const Point_3& p) const
   {
     int res = pl.side_of(p);
     if (res == 1) return true;
     return false;
   }

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     CGAL::Orientation ori = s.orientation();  
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     bool res;
     if (d < sq) res = true;
     else res = false;
     
     // testen ...
     if (ori == CLOCKWISE) res = !res;
     
     return res; 
  }
#else
   bool operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     int res = LEDA_NAMESPACE_NAME::side_of_sphere(s.point1(),s.point2(),s.point3(),s.point4() ,p);
     if (res == 1) return true;
     return false;
   }
#endif
   
//  welche Seite von Tetrahedron ist positive, welche ist die negative ? Dok. ist etwas unklar ...   

   bool operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
       // orientation * bounded side
       int ori = LEDA_NAMESPACE_NAME::orientation(s.point1(), s.point2(), s.point3(), s.point4());
       
       if (ori != 0) {
       
         Bounded_side reg = leda_support::bounded_side(s,p);
	 
	 if (((CGAL::Oriented_side) (reg*ori)) == ON_POSITIVE_SIDE) return true;
	 else return false;
       }
       
       return false;      
   }
   
};

template<class K>
class Predicate_leda_d3_rat_has_on_negative_side {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Plane_3      Plane_3;   
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::FT           FT;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

   bool operator()(const Plane_3& pl, const Point_3& p) const
   {
     int res = pl.side_of(p);
     if (res == -1) return true;
     return false;
   }

#if defined(CGAL_COMPATIBLE_SPHERES)
  bool operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     CGAL::Orientation ori = s.orientation();  
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     bool res;
     if (d > sq) res = true;
     else res = false;
     
     // testen ...
     if (ori == CLOCKWISE) res = !res;
     
     return res; 
  }
#else
   bool operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     int res = LEDA_NAMESPACE_NAME::side_of_sphere(s.point1(),s.point2(),s.point3(),s.point4() ,p);
     if (res == -1) return true;
     return false;
   }
#endif
   
//  welche Seite von Tetrahedron ist positive, welche ist die negative ? Dok. ist etwas unklar ...   

   bool operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
       // orientation * bounded side
       int ori = LEDA_NAMESPACE_NAME::orientation(s.point1(), s.point2(), s.point3(), s.point4());
       
       if (ori != 0) {
       
         Bounded_side reg = leda_support::bounded_side(s,p);
	 
	 if (((CGAL::Oriented_side) (reg*ori)) == ON_NEGATIVE_SIDE) return true;
	 else return false;
       }
       
       return false;      
   }
   
};

template<class K>
class Predicate_leda_d3_rat_oriented_side {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Plane_3      Plane_3;   
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::FT           FT;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Oriented_side       result_type;

   Oriented_side operator()(const Plane_3& pl, const Point_3& p) const
   {
     return (Oriented_side) (pl.side_of(p));
   }

#if defined(CGAL_COMPATIBLE_SPHERES)
  CGAL::Oriented_side operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s,
                                 const Point_3& p) const
  {
     Point_3 center = s.center();
     CGAL::Orientation ori = s.orientation();  
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     if (d == sq) return CGAL::ON_ORIENTED_BOUNDARY;
     
     bool res;
     if (d < sq) res = true;
     else res = false;
     
     // testen ...
     if (ori == CLOCKWISE) res = !res;
     
     if (res) return CGAL::ON_POSITIVE_SIDE;
     return CGAL::ON_NEGATIVE_SIDE; 
  }
#else   
   Oriented_side operator()(const leda_d3_rat_sphere& s, const Point_3& p) const
   {
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();     
     return (Oriented_side) (LEDA_NAMESPACE_NAME::side_of_sphere(p1,p2,p3,p4,p));
   } 
#endif

//  welche Seite von Tetrahedron ist positive, welche ist die negative ? Dok. ist etwas unklar ...  
//  precondition seems to be that the tetrahedron is not degenerate ...
 
   Oriented_side operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
       // orientation * bounded side
       int ori = LEDA_NAMESPACE_NAME::orientation(s.point1(), s.point2(), s.point3(), s.point4());
       
       if (ori != 0) {
       
         Bounded_side reg = leda_support::bounded_side(s,p);
	 
	 return (CGAL::Oriented_side) (reg*ori);
       }
       
       return ON_ORIENTED_BOUNDARY;     
   }
  
};

template<class K>
class Predicate_leda_d3_rat_bounded_side {

  typedef typename K::Point_3      Point_3; 
  typedef typename K::Tetrahedron_3  Tetrahedron_3;
  typedef typename K::Iso_cuboid_3 Iso_cuboid_3;        
  typedef typename K::FT           FT;  
  
  typedef typename K::Point_3   __My_Point_3;  

public:
  typedef Arity_tag< 2 > Arity;
  typedef Bounded_side   result_type;

#if defined(CGAL_COMPATIBLE_SPHERES)
  Bounded_side operator()(const LEDA_NAMESPACE_NAME::cgal_d3_rat_sphere& s, const Point_3& p) const
  {
     Point_3 center = s.center();
     FT sq      = s.squared_radius();
     FT d       = center.sqr_dist(p);
     
     if (d==sq) return CGAL::ON_BOUNDARY;
     if (d <sq) return CGAL::ON_BOUNDED_SIDE;
     return CGAL::ON_UNBOUNDED_SIDE;
  }
#else
   Bounded_side operator()(const leda_d3_rat_sphere& s, const Point_3& test) const
   {
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();     
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();     
       
     int res = LEDA_NAMESPACE_NAME::region_of_sphere(p1,p2,p3,p4,test);
     if (res==0) return CGAL::ON_BOUNDARY;
     if (res==-1) return CGAL::ON_UNBOUNDED_SIDE;
     return CGAL::ON_BOUNDED_SIDE;     
   }
#endif   
  
   Bounded_side operator()(const Tetrahedron_3& s, const Point_3& p) const
   {
     // inside returns true if p is inside or on s ...
     
     bool inside = s.in_simplex(p);
     if (! inside) return CGAL::ON_UNBOUNDED_SIDE;
     
     // is the point ON the simplex ???
     Point_3 p1 = s.point1();
     Point_3 p2 = s.point2();
     Point_3 p3 = s.point3();
     Point_3 p4 = s.point4();   
     
     if (LEDA_NAMESPACE_NAME::coplanar(p1,p2,p3,p) ||  LEDA_NAMESPACE_NAME::coplanar(p1,p2,p4,p) ||
         LEDA_NAMESPACE_NAME::coplanar(p2,p3,p4,p) ||  LEDA_NAMESPACE_NAME::coplanar(p3,p1,p4,p)) return CGAL::ON_BOUNDARY; 
     
     return CGAL::ON_BOUNDED_SIDE;
   }   
   
   Bounded_side operator()(const Iso_cuboid_3& ic, const Point_3& p) const
   {
     Point_3 min = ic.vertex(0);
     Point_3 max = ic.vertex(7);
     
     // compare with x/y/z coordinates ...
     // we need at least one zero in the cmp results;
     int res1 = __My_Point_3::cmp_x(p,min);
     int res2 = __My_Point_3::cmp_x(p,max);
     
     if (res1 == -1 || res2 == 1) return CGAL::ON_UNBOUNDED_SIDE;
     
     int res3 = __My_Point_3::cmp_y(p,min);
     int res4 = __My_Point_3::cmp_y(p,max);     
     
     if (res3 == -1 || res4 == 1) return CGAL::ON_UNBOUNDED_SIDE;
     
     int res5 = __My_Point_3::cmp_z(p,min);
     int res6 = __My_Point_3::cmp_z(p,max);        
     
     if (res5 == -1 || res6 == 1) return CGAL::ON_UNBOUNDED_SIDE;     
          
     // we need at least one zero in the comparison results ...	  
     if (res1==0 || res2==0 || res3==0 || res4==0 || res5==0 || res6==0) return CGAL::ON_BOUNDARY;  	  
	  
     return CGAL::ON_BOUNDED_SIDE;         
   }        
     
};

template<class K>
class Predicate_leda_d3_rat_are_ordered_along_line {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Segment_3    Segment_3;    

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

  bool operator()(const Point_3& p, const Point_3& q, const Point_3& r) const
  { return Segment_3(p,r).contains(q); }
};

template<class K>
class Predicate_leda_d3_rat_are_strictly_ordered_along_line {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Segment_3    Segment_3;  

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

  bool operator()(const Point_3& p, const Point_3& q, const Point_3& r) const
  {
      return (Segment_3(p,r).contains(q) && ( q != p ) && ( q != r ));   
  }
};

// these two predicates could probably be optimized further ....
// (because the collinearity check in contains is not necessary -
// collinearity is a precondition) 

template<class K>
class Predicate_leda_d3_rat_collinear_are_ordered_along_line {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Segment_3    Segment_3;  

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

  bool operator()(const Point_3& p, const Point_3& q, const Point_3& r) const
  {
      return Segment_3(p,r).contains(q);  
  }
};

template<class K>
class Predicate_leda_d3_rat_collinear_are_strictly_ordered_along_line {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Segment_3    Segment_3;  

public:
  typedef Arity_tag< 3 > Arity;
  typedef bool       result_type;

  bool operator()(const Point_3& p, const Point_3& q, const Point_3& r) const
  {
      return (Segment_3(p,r).contains(q) && ( q != p ) && ( q != r ));   
  }
};

template<class K>
class Predicate_leda_d3_rat_do_intersect {

  typedef typename K::Point_3      Point_3;
  typedef typename K::Line_3       Line_3;
  typedef typename K::Plane_3      Plane_3; 
  typedef typename K::Ray_3        Ray_3;
  typedef typename K::Segment_3    Segment_3;     
  typedef typename K::FT           FT;

public:
  typedef Arity_tag< 2 > Arity;
  typedef bool       result_type;

// intersection tests with 3d plane ...

  bool operator()(const Plane_3& pl, const Plane_3& pl2) const
  {
     if (pl.parallel(pl2)) return false;
     
     // non-parallel planes always intersect ...
     return true;
  }
  
  bool operator()(const Plane_3& pl, const Line_3& l) const
  {
    Point_3 p1 = l.point1();
    Point_3 p2 = l.point2();
  
    int s1 = pl.side_of(p1);
    int s2 = pl.side_of(p2);
    
    if (s1==0 && s2==0) return true;
    if (s1 != s2) return true;  
    
    // the points p1 and p2 are on one side of the plane ...
    // so compare the distance of them; if it is not equal, 
    // we have an intersection ...
    // later we should have a filtered distance comparison predicate
    
    if (pl.sqr_dist(p1) == pl.sqr_dist(p2)) return false; // no intersection
    
    return true;
  }
  
  bool operator()(const Line_3& l, const Plane_3& pl) const
  {
     return this->operator()(pl, l);
  }  

  bool operator()(const Plane_3& pl, const Ray_3& r) const
  {
    Point_3 p1 = r.point1(); // source
    Point_3 p2 = r.point2(); // target
  
    int s1 = pl.side_of(p1);
    int s2 = pl.side_of(p2);
    
    if (s1==0 && s2==0) return true;
    if (s1 != s2) return true;  
    
    // source and other point are on same side ...
    FT r1 = pl.sqr_dist(p1);
    FT r2 = pl.sqr_dist(p2); 
    
    if (r1 <= r2) return false; // the ray is going away from the plane
    return true;  
  }  

  bool operator()(const Ray_3& r, const Plane_3& pl) const
  {
     return this->operator()(pl, r);  
  }  

  bool operator()(const Plane_3& pl, const Segment_3& s) const
  {
    int s1 = pl.side_of(s.source());
    int s2 = pl.side_of(s.target());
    
    if (s1==0 && s2==0) return true;
    if (s1 != s2) return true;
    return false;
  }  
  
  bool operator()(const Segment_3& s, const Plane_3& pl) const
  {
     return this->operator()(pl, s);    
  }  
  
};


CGAL_END_NAMESPACE

#endif






