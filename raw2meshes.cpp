#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Labeled_image_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>
// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3,K> Mesh_domain;
// Triangulation
#ifdef CGAL_CONCURRENT_MESH_3
  typedef CGAL::Mesh_triangulation_3<
    Mesh_domain,
    CGAL::Kernel_traits<Mesh_domain>::Kernel, // Same as sequential
    CGAL::Parallel_tag                        // Tag to activate parallelism
  >::type Tr;
#else
  typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
#endif
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
// Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;
// To avoid verbose function and named parameters call
using namespace CGAL::parameters;
int main(int argc, char* argv[])
{
  const char* fname = argv[1];
  const unsigned int sx = atoi(argv[2]);
  const unsigned int sy = atoi(argv[3]);
  const unsigned int sz = atoi(argv[4]);
  // Loads image
  CGAL::Image_3 image;
  if(!image.read_raw(fname,sx,sy,sz)){
    std::cerr << "Error: Cannot read file " <<  fname << std::endl;
    return EXIT_FAILURE;
  }
  // Domain
  Mesh_domain domain(image);
  // Mesh criteria
  Mesh_criteria criteria(facet_angle=30, facet_size=1, facet_distance=3,
                         cell_radius_edge_ratio=3, cell_size=8);
  // Meshing
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria);

 //Labels
 std::set<C3t3::Subdomain_index> labelset;
 for(unsigned int i = 0; i< sx*sy*sz; i++)
    labelset.insert( ((unsigned char*)image.data() )[ i ] );
  std::cout << "Found "<< labelset.size()<< " labels."<<std::endl;

  // Output
  for(std::set<C3t3::Subdomain_index>::const_iterator it=labelset.begin();
      it != labelset.end(); ++it )
  {
    std::string name= "out_";
    name += std::to_string( *it );
    name += ".off";
    std::ofstream off_file(name);
    c3t3.output_boundary_to_off(off_file, *it );
  }
  return 0;
}
