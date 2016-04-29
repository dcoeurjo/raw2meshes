#David Coeurjolly <david.coeurjolly@liris.cnrs.fr>
# -----------------------------------------------------------------------------
# Project definition
# -----------------------------------------------------------------------------
PROJECT(raw2meshes)

cmake_minimum_required (VERSION 2.6)



find_package(CGAL COMPONENTS Core ImageIO)
include( ${CGAL_USE_FILE} )
include( CGAL_CreateSingleSourceCGALProgram )

SET(SRCS
  raw2meshes
)



IF (CGAL_FOUND)
  create_single_source_cgal_program( "raw2meshes.cpp" )
ENDIF()
