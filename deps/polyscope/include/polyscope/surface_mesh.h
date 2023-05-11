// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

#include <memory>
#include <vector>

#include "polyscope/affine_remapper.h"
#include "polyscope/color_management.h"
#include "polyscope/gl/gl_utils.h"
#include "polyscope/polyscope.h"
#include "polyscope/standardize_data_array.h"
#include "polyscope/structure.h"
#include "polyscope/surface_mesh_enums.h"
#include "polyscope/surface_mesh_quantity.h"

// Alllll the quantities
#include "polyscope/surface_color_quantity.h"
#include "polyscope/surface_count_quantity.h"
#include "polyscope/surface_distance_quantity.h"
#include "polyscope/surface_graph_quantity.h"
#include "polyscope/surface_parameterization_enums.h"
#include "polyscope/surface_parameterization_quantity.h"
#include "polyscope/surface_scalar_quantity.h"
#include "polyscope/surface_vector_quantity.h"
//#include "polyscope/surface_selection_quantity.h"
//#include "polyscope/surface_subset_quantity.h"


namespace polyscope {

// Forward declarations for quantities
class SurfaceVertexColorQuantity;
class SurfaceFaceColorQuantity;
class SurfaceVertexCountQuantity;
class SurfaceVertexIsolatedScalarQuantity;
class SurfaceFaceCountQuantity;
class SurfaceDistanceQuantity;
class SurfaceGraphQuantity;
class SurfaceCornerParameterizationQuantity;
class SurfaceVertexParameterizationQuantity;
class SurfaceVertexScalarQuantity;
class SurfaceFaceScalarQuantity;
class SurfaceEdgeScalarQuantity;
class SurfaceHalfedgeScalarQuantity;
class SurfaceVertexVectorQuantity;
class SurfaceFaceVectorQuantity;
class SurfaceVertexIntrinsicVectorQuantity;
class SurfaceFaceIntrinsicVectorQuantity;
class SurfaceOneFormIntrinsicVectorQuantity;


template <> // Specialize the quantity type
struct QuantityTypeHelper<SurfaceMesh> {
  typedef SurfaceMeshQuantity type;
};


// === The grand surface mesh class

class SurfaceMesh : public QuantityStructure<SurfaceMesh> {
public:
  typedef SurfaceMeshQuantity QuantityType;

  // === Member functions ===

  // Construct a new surface mesh structure
  SurfaceMesh(std::string name, const std::vector<glm::vec3>& vertexPositions,
              const std::vector<std::vector<size_t>>& faceIndices);

  // Build the imgui display
  virtual void buildCustomUI() override;
  virtual void buildCustomOptionsUI() override;
  virtual void buildPickUI(size_t localPickID) override;

  // Render the the structure on screen
  virtual void draw() override;

  // Render for picking
  virtual void drawPick() override;

  // A characteristic length for the structure
  virtual double lengthScale() override;

  // Axis-aligned bounding box for the structure
  virtual std::tuple<glm::vec3, glm::vec3> boundingBox() override;
  virtual std::string typeName() override;

  // === Quantity-related

  // clang-format off

  // = Scalars (expect scalar array)
  template <class T>
  SurfaceVertexScalarQuantity* addVertexScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD);
  template <class T>
  SurfaceFaceScalarQuantity* addFaceScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD);
  template <class T>
  SurfaceEdgeScalarQuantity* addEdgeScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD);
  template <class T>
  SurfaceHalfedgeScalarQuantity* addHalfedgeScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD);

  // = Distance (expect scalar array)
  template <class T>
  SurfaceDistanceQuantity* addVertexDistanceQuantity(std::string name, const T& data);
  template <class T>
  SurfaceDistanceQuantity* addVertexSignedDistanceQuantity(std::string name, const T& data);

  // = Colors (expect vec3 array)
  template <class T>
  SurfaceVertexColorQuantity* addVertexColorQuantity(std::string name, const T& data);
  template <class T>
  SurfaceFaceColorQuantity* addFaceColorQuantity(std::string name, const T& data);

  // = Counts/Values on isolated vertices (expect index/value pairs)
  SurfaceVertexCountQuantity* addVertexCountQuantity(std::string name, const std::vector<std::pair<size_t, int>>& values);
  SurfaceFaceCountQuantity* addFaceCountQuantity(std::string name, const std::vector<std::pair<size_t, int>>& values);
  SurfaceVertexIsolatedScalarQuantity* addVertexIsolatedScalarQuantity(std::string name, const std::vector<std::pair<size_t, double>>& values);

  // = Subsets (expect char array)
  // template <class T>
  // void addEdgeSubsetQuantity(std::string name, const T& subset);

  // = Vectors (expect vector array, inner type must be indexable with correct dimension (3 for extrinsic, 2 for intrinsic)
  template <class T>
  SurfaceVertexVectorQuantity* addVertexVectorQuantity(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD);
  template <class T>
  SurfaceVertexVectorQuantity* addVertexVectorQuantity2D(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD);
  template <class T>
  SurfaceFaceVectorQuantity* addFaceVectorQuantity(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD);
  template <class T>
  SurfaceFaceVectorQuantity* addFaceVectorQuantity2D(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD);
  template <class T>
  SurfaceFaceIntrinsicVectorQuantity* addFaceIntrinsicVectorQuantity(std::string name, const T& vectors, int nSym = 1, VectorType vectorType = VectorType::STANDARD);
  template <class T>
  SurfaceVertexIntrinsicVectorQuantity* addVertexIntrinsicVectorQuantity(std::string name, const T& vectors, int nSym = 1, VectorType vectorType = VectorType::STANDARD);
  template <class T, class O>
  SurfaceOneFormIntrinsicVectorQuantity* addOneFormIntrinsicVectorQuantity(std::string name, const T& data, const O& orientations);

  // = Parameterizations
  template <class T>
  SurfaceCornerParameterizationQuantity* addParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::UNIT);
  template <class T>
  SurfaceVertexParameterizationQuantity* addVertexParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::UNIT);
  template <class T>
  SurfaceVertexParameterizationQuantity* addLocalParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::WORLD);


  // = Misc quantities
  template <class P, class E>
  SurfaceGraphQuantity* addSurfaceGraphQuantity(std::string name, const P& nodes, const E& edges);
  template <class P, class E>
  SurfaceGraphQuantity* addSurfaceGraphQuantity2D(std::string name, const P& nodes, const E& edges);
  template <class P>
  SurfaceGraphQuantity* addSurfaceGraphQuantity(std::string name, const std::vector<P>& paths);
  template <class P>
  SurfaceGraphQuantity* addSurfaceGraphQuantity2D(std::string name, const std::vector<P>& paths);

  // = I/O Selections
  template <class T>
  void addVertexSelectionQuantity(std::string name, const T& initialMembership);
  // void addInputCurveQuantity(std::string name);

  // clang-format on


  // === Make a one-time selection
  long long int selectVertex();
  // size_t selectFace();

  // === Mutate
  template <class V>
  void updateVertexPositions(const V& newPositions);
  template <class V>
  void updateVertexPositions2D(const V& newPositions2D);


  // === Indexing conventions

  // Permutation arrays. Empty == default ordering
  std::vector<size_t> vertexPerm;
  std::vector<size_t> facePerm;
  std::vector<size_t> edgePerm;
  std::vector<size_t> halfedgePerm;
  std::vector<size_t> cornerPerm;

  // Set permutations
  template <class T>
  void setVertexPermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setFacePermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setEdgePermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setHalfedgePermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setCornerPermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setAllPermutations(const std::array<std::pair<T, size_t>, 5>& perms);

  // Get the expected data length, either using the default convention or a permutation as above
  size_t vertexDataSize;
  size_t faceDataSize;
  size_t edgeDataSize;
  size_t halfedgeDataSize;
  size_t cornerDataSize;


  // === Helpers
  void fillGeometryBuffers(gl::GLProgram& p);
  void setShadeStyle(ShadeStyle newShadeStyle);


  // === Manage the mesh itself

  // Core data
  std::vector<glm::vec3> vertices;
  std::vector<std::vector<size_t>> faces;

  // Derived indices
  std::vector<std::vector<size_t>> edgeIndices;
  std::vector<std::vector<size_t>> halfedgeIndices;

  // Counts
  size_t nVertices() const { return vertices.size(); }
  size_t nFaces() const { return faces.size(); }

  size_t nFacesTriangulationCount = 0;
  size_t nFacesTriangulation() const { return faces.size(); }

  size_t nEdgesCount = 0;
  size_t nEdges() const { return nEdgesCount; }

  size_t nCornersCount = 0; // = nHalfedges = sum face degree
  size_t nCorners() const { return nCornersCount; }
  size_t nHalfedges() const { return nCornersCount; }

  // Derived geometric quantities
  std::vector<glm::vec3> faceNormals;
  std::vector<glm::vec3> vertexNormals;
  std::vector<double> faceAreas;
  std::vector<double> vertexAreas;
  std::vector<double> edgeLengths;

  std::vector<std::array<glm::vec3, 2>> faceTangentSpaces;
  std::vector<std::array<glm::vec3, 2>> vertexTangentSpaces;


  // = Mesh helpers
  void computeCounts();       // call to populate counts and indices
  void computeGeometryData(); // call to populate normals/areas/lengths

  // if there are no tangent spaces, builds the default ones
  void ensureHaveFaceTangentSpaces();
  void ensureHaveVertexTangentSpaces();

  // Set tangent space coordinates for vertices
  template <class T>
  void setVertexTangentBasisX(const T& vectors);
  template <class T>
  void setVertexTangentBasisX2D(const T& vectors);

  // Set tangent space coordinates for faces
  template <class T>
  void setFaceTangentBasisX(const T& vectors);
  template <class T>
  void setFaceTangentBasisX2D(const T& vectors);


  // Set tangent space coordinates for faces


  // === Member variables ===
  static const std::string structureTypeName;

  // Picking helpers
  // One of these will be non-null on return
  // void getPickedElement(size_t localPickID, size_t& vOut, size_t& fOut, size_t& eOut, size_t& heOut);

  // Returns the face ands coordinates in that face of the last pick. fOut == FacePtr() if not in any face. Note that
  // you may needed to update the pick data, beacuse this uses mouse coordinates from the current state but possibly
  // old pick lookup results.
  // void getPickedFacePoint(FacePtr& fOut, glm::vec3& baryCoordOut);

  // Visualization settings
  glm::vec3 baseColor;
  glm::vec3 surfaceColor;
  glm::vec3 edgeColor{0., 0., 0.};
  float edgeWidth = 0.0;

private:
  // Visualization settings
  ShadeStyle shadeStyle = ShadeStyle::FLAT;

  // Do setup work related to drawing, including allocating openGL data
  void prepare();
  void preparePick();
  void prepareWireframe();
  void geometryChanged(); // call whenever geometry changed

  // Picking-related
  // Order of indexing: vertices, faces, edges, halfedges
  // Within each set, uses the implicit ordering from the mesh data structure
  // These starts are LOCAL indices, indexing elements only with the mesh
  size_t facePickIndStart, edgePickIndStart, halfedgePickIndStart;
  void buildVertexInfoGui(size_t vInd);
  void buildFaceInfoGui(size_t fInd);
  void buildEdgeInfoGui(size_t eInd);
  void buildHalfedgeInfoGui(size_t heInd);

  // Gui implementation details

  // Drawing related things
  std::unique_ptr<gl::GLProgram> program;
  std::unique_ptr<gl::GLProgram> pickProgram;
  std::unique_ptr<gl::GLProgram> wireframeProgram;

  // === Helper functions

  // Initialization work
  void initializeMeshTriangulation();

  void fillGeometryBuffersSmooth(gl::GLProgram& p);
  void fillGeometryBuffersFlat(gl::GLProgram& p);
  void fillGeometryBuffersWireframe(gl::GLProgram& p);
  glm::vec2 projectToScreenSpace(glm::vec3 coord);
  // bool screenSpaceTriangleTest(size_t fInd, glm::vec2 testCoords, glm::vec3& bCoordOut);


  // clang-format off

  // === Quantity adders

  SurfaceVertexColorQuantity* addVertexColorQuantityImpl(std::string name, const std::vector<glm::vec3>& colors);
  SurfaceFaceColorQuantity* addFaceColorQuantityImpl(std::string name, const std::vector<glm::vec3>& colors);
  SurfaceVertexCountQuantity* addVertexCountQuantityImpl(std::string name, const std::vector<std::pair<size_t, int>>& values);
  SurfaceVertexIsolatedScalarQuantity* addVertexIsolatedScalarQuantityImpl(std::string name, const std::vector<std::pair<size_t, double>>& values);
  SurfaceFaceCountQuantity* addFaceCountQuantityImpl(std::string name, const std::vector<std::pair<size_t, int>>& values);
  SurfaceDistanceQuantity* addVertexDistanceQuantityImpl(std::string name, const std::vector<double>& data);
  SurfaceDistanceQuantity* addVertexSignedDistanceQuantityImpl(std::string name, const std::vector<double>& data);
  SurfaceGraphQuantity* addSurfaceGraphQuantityImpl(std::string name, const std::vector<glm::vec3>& nodes, const std::vector<std::array<size_t, 2>>& edges);
  SurfaceCornerParameterizationQuantity* addParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexParameterizationQuantity* addVertexParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexParameterizationQuantity* addLocalParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexScalarQuantity* addVertexScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceFaceScalarQuantity* addFaceScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceEdgeScalarQuantity* addEdgeScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceHalfedgeScalarQuantity* addHalfedgeScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceVertexVectorQuantity* addVertexVectorQuantityImpl(std::string name, const std::vector<glm::vec3>& vectors, VectorType vectorType);
  SurfaceFaceVectorQuantity* addFaceVectorQuantityImpl(std::string name, const std::vector<glm::vec3>& vectors, VectorType vectorType);
  SurfaceFaceIntrinsicVectorQuantity* addFaceIntrinsicVectorQuantityImpl(std::string name, const std::vector<glm::vec2>& vectors, int nSym, VectorType vectorType);
  SurfaceVertexIntrinsicVectorQuantity* addVertexIntrinsicVectorQuantityImpl(std::string name, const std::vector<glm::vec2>& vectors, int nSym, VectorType vectorType);
  SurfaceOneFormIntrinsicVectorQuantity* addOneFormIntrinsicVectorQuantityImpl(std::string name, const std::vector<double>& data, const std::vector<char>& orientations);

  // === Helper implementations

  void setVertexTangentBasisXImpl(const std::vector<glm::vec3>& vectors);
  void setFaceTangentBasisXImpl(const std::vector<glm::vec3>& vectors);
  // clang-format on
};

// Register functions
template <class V, class F>
SurfaceMesh* registerSurfaceMesh(std::string name, const V& vertexPositions, const F& faceIndices,
                                 bool replaceIfPresent = true);
template <class V, class F>
SurfaceMesh* registerSurfaceMesh2D(std::string name, const V& vertexPositions, const F& faceIndices,
                                   bool replaceIfPresent = true);
template <class V, class F, class P>
SurfaceMesh* registerSurfaceMesh(std::string name, const V& vertexPositions, const F& faceIndices,
                                 const std::array<std::pair<P, size_t>, 5>& perms, bool replaceIfPresent = true);


// Shorthand to get a mesh from polyscope
inline SurfaceMesh* getSurfaceMesh(std::string name = "");


} // namespace polyscope

#include "polyscope/surface_mesh.ipp"
