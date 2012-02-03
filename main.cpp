/*!
 * \file main.cpp
 * \author Simon Coakley
 * \date 2012
 * \copyright Copyright (c) 2012 University of Sheffield
 * \brief Contains \c main()
 * \details Starts the program and shows the \c MainWindow
 */
#include <QtGui/QApplication>
#include "./mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*!
 * \mainpage FLAME Editor
 *
 * \section OVERVIEW Overview
 * The FLAME Editor is a generic visual editor for FLAME models that use the FLAME framework (http://www.flame.ac.uk).
 *
 * \section DESIGN Design
 * The GUI is split into 4 main sections:
 * -# The model tree
 * -# The memory table
 * -# The stategraph graphics scene
 * -# The transition functions table
 *
 * \section DATASTRUCT Data Structures
 * -# The model tree class \c MachineTree is an implementation of \c QAbstractItemModel
 *    - which holds objects of type \c Machine in a tree structure
 * -# The memory table class \c MemoryModel is an implementation of \c QAbstractTableModel
 *    - which holds objects of type \c Variable in a table structure
 * -# The stategraph graphics scene class \c MachineScene is an implementation of \c QGraphicsScene
 *    - which holds objects of type \c GraphicsItem and \c Arrow
 * -# The transition functions table class \c MachineModel is an implementation of \c QAbstractTableModel
 *    - which holds objects of type \c Transition
 *
 * The \c Machine class is used to represent a FLAME model and all of its main elements:
 * - model - either the main model or any sub model
 * - agent
 * - message
 * - environment - an element that contains any environment elements of the model, which include:
 * - time unit
 * - data type
 * - function file
 *
 * The \c Machine class can contain:
 * - \c MachineModel - to hold a table of agent transitions
 * - \c MemoryModel - to hold a memory table for agents, messages, environment constants,
 * data types and model version/author/description fields
 * - \c MachineScene - to hold a graph of agent transitions
 * - \c TimeUnit - to hold a time unit
 *
 * The \c Variable class is used to represent agent/message memory variables,
 * environment constants, data type variables and model version/author/description fields and contains:
 * - type - the data type of the variable/constant
 * - name - the name of the variable/constant or the model attribute label
 * - description - the variable/constant description or the model attribute value
 *
 * The \c GraphicsItem class is used to represent agent states, transition functions or message types.
 *
 * The \c Arrow class is used to represent an arrow between two objects of type \c GraphicsItem.
 *
 * The \c Transition class is used to represent agent state transitions and contains:
 * - \c State current state
 * - \c Communication input
 * - \c Mpre pre condition
 * - function name
 * - \c Mpost post condition - currently unused
 * - \c Communication output
 * - \c State next state
 *
 * The \c Communication class is used to represent incoming and outgoing message communication and contains:
 * - \c MessageModel which is an implementation of \c QAbstractTableModel and holds a table of:
 *   - \c MessageComm which contains:
 *     - message type
 *     - \c Condition - a filter on incoming messages
 *     - \c MessageSort - a sort on incoming messages
 *
 * \section DELEGATES Object Delegates
 * A delegate class handles the interaction of the user with editing objects within cells of a tree or table.
 * A dialog class can be associated with the editing of certain objects.
 *
 * -# The model tree is implemented in \c MachineTree.
 * It holds objects of type \c Machine.
 * It has associated delegates:
 * - \c MachineTreeDelegate. This calls dialogs to edit certain objects:
 *   - \c TimeUnitDialog
 * -# The memory model is implemented in \c MemoryModel.
 * It holds objects of type \c Variable.
 * It has associated delegates:
 * - \c TextEditDelegate
 * - \c DataTypeDelegate
 * - \c LineEditDelegate
 * -# The transition functions table is implemented in \c MachineModel.
 * It holds objects of type \c Transition.
 * It has associated delegates:
 * - \c LineEditDelegate
 * - \c CommDelegate
 *   - \c CommDialog
 *     - \c MessageDelegate
 *     - \c MpreDelegate
 *       - \c MpreDialog
 *     - \c SortDelegate
 *       - \c SortDialog
 * - \c MpreDelegate
 *   - \c MpreDialog
 * - \c MpostDelegate
 *   - \c MpostDialog
 */
