// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef HEPREP_HEPREPFACTORY_H
#define HEPREP_HEPREPFACTORY_H 1

// Copyright 2000-2002, FreeHEP.

#include <iostream>
#include <string>

namespace HEPREP {

class HepRep;
class HepRepAction;
class HepRepInstance;
class HepRepInstanceTree;
class HepRepPoint;
class HepRepReader;
class HepRepTreeID;
class HepRepType;
class HepRepTypeTree;
class HepRepWriter;

/**
 * HepRepFactory interface.
 *
 * @author Mark Donszelmann
 */
class HepRepFactory {

public: 
    /// Destructor.
    virtual ~HepRepFactory() { /* nop */; }

    /**
     * Creates a HepRepReader from a stream.
     *
     * @param in input stream.
     */
    virtual HepRepReader * createHepRepReader(std::istream * in) = 0;

    /**
     * Creates a HepRepReader from a file name.
     *
     * @param inputFileName file name.
     */
    virtual HepRepReader * createHepRepReader(std::string inputFileName) = 0;

    /**
     * Creates a HepRepWriter.
     *
     * @param out output stream.
     * @param randomAccess create a writer in a format that will allow random access (may be ignored).
     * @param compress create a writer that uses compression (may be ignored).
     */
    virtual HepRepWriter * createHepRepWriter(std::ostream * out, bool randomAccess, bool compress) = 0;

    /**
     * Creates a HepRepPoint.
     *
     * @param instance to add the point to.
     * @param x x coordinate of point.
     * @param y y coordinate of point.
     * @param z z coordinate of point.
     */
    virtual HepRepPoint * createHepRepPoint(HepRepInstance * instance, double x, double y, double z) = 0;

    /**
     * Creates a HepRepInstance.
     *
     * @parent to add the instance to.
     * @type type the associated type.
     */
    virtual HepRepInstance * createHepRepInstance(HepRepInstance * parent, HepRepType * type) = 0;

    /**
     * Creates a HepRepInstance.
     *
     * @parent to add the instance to.
     * @type type the associated type.
     */
    virtual HepRepInstance * createHepRepInstance(HepRepInstanceTree * parent, HepRepType * type) = 0;

    /**
     * Creates a HepRepTreeID.
     *
     * @param name of the treeID.
     * @param version of the treeID.
     * @param qualifier of the treeID.
     */
    virtual HepRepTreeID * createHepRepTreeID(std::string name, std::string version, std::string qualifier = "top-level") = 0;

    /**
     * Creates a HepRepAction.
     *
     * @param name of the action.
     * @param expression of the action.
     */
    virtual HepRepAction * createHepRepAction(std::string name, std::string expression) = 0;

    /**
     * Creates a HepRepInstanceTree.
     * <p>
     * The tree needs to be added to the HepRep.
     *
     * @param name of the instancetree.
     * @param version of the instancetree.
     * @param typeTree associated typetree.
     */
    virtual HepRepInstanceTree * createHepRepInstanceTree(std::string name, std::string version, HepRepTreeID * typeTree) = 0;

    /**
     * Creates a HepRepType.
     *
     * @param parent to add this type to.
     * @param name of the type to create.
     */
    virtual HepRepType * createHepRepType(HepRepTypeTree * parent, std::string name) = 0;

    /**
     * Creates a HepRepType.
     *
     * @param parent to add this type to.
     * @param name of the type to create.
     */
    virtual HepRepType * createHepRepType(HepRepType * parent, std::string name) = 0;

    /**
     * Creates a HepRepTypeTree.
     * <p>
     * The tree needs to be added to the HepRep.
     *
     * @param treeID to name the tree being created.
     */
    virtual HepRepTypeTree * createHepRepTypeTree(HepRepTreeID * treeID) = 0;

    /**
     * Creates a HepRep.
     *
     */
    virtual HepRep * createHepRep() = 0;
}; // class
} // namespace HEPREP
#endif /* ifndef HEPREP_HEPREPFACTORY_H */
