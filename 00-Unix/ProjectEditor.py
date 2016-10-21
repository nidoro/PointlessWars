import sys
import re
import os

SOURCES_DIR = '../src'
HEADERS_DIR = '../include'
THIS_DIR = os.path.dirname(os.path.abspath(__file__)) 

def newClass(classname, parentname = None):
    newHeader(classname + '.h', classname, parentname)
    newSource(classname + '.cpp')
    print(" New class created: " + classname)
    return 

def newHeader(filename, classname = None, parentname = None):
    path = THIS_DIR + '/' + HEADERS_DIR + '/' + filename  
    if os.path.exists(path):
        print(" Error: file already exists: " + path)
        user_response = input(" Replace file? (yes/no) ")
        if user_response.upper() != "YES":
            return

    createHeaderFile(filename, classname, parentname) 
    print(" New header created: " + filename)
    return

def newSource(filename):
    path = THIS_DIR + '/' + SOURCES_DIR + '/' + filename  
    file_exists = False
    if os.path.exists(path):
        file_exists = True
        print(" Error: file already exists: " + path)
        user_response = input(" Replace file? (yes/no) ")
        if user_response.upper() != "YES":
            return

    createSourceFile(filename)
    print(" New source created: " + filename)

    # EDIT CMakeLists.txt
    removeSource(filename)
    cmk = open(THIS_DIR + '/' + 'CMakeLists.txt', 'r')
    cmkLines = cmk.readlines()
    cmkNewLines = []
    for line in cmkLines:
        cmkNewLines += line
        words = line.split()
        if len(words) == 0: continue 
        if words[0] == "add_executable":
           cmkNewLines += '\t' + SOURCES_DIR + '/' + filename + '\n'

    cmk.close()
    
    cmkOut = open(THIS_DIR + '/' + 'CMakeLists.txt', 'w')
    for line in cmkNewLines:
        cmkOut.write(line)

    cmkOut.close()
    return

def removeSource(filename):
    cmk = open(THIS_DIR + '/' + 'CMakeLists.txt', 'r')
    cmkLines = cmk.readlines()
    cmkNewLines = []
    removed = False
    for line in cmkLines:
        words = line.split()
        if len(words) == 0: continue 
        if words[0] != SOURCES_DIR + '/' + filename:
            cmkNewLines += line
        else:
            removed = True
    
    cmk.close()
        
    if removed:
        cmkOut = open(THIS_DIR + '/' + 'CMakeLists.txt', 'w')
        for line in cmkNewLines:
            cmkOut.write(line)
        
        print(" Source file removed: " + filename)
        cmkOut.close()
    
def createSourceFile(filename):
    path = THIS_DIR + '/' + SOURCES_DIR + '/' + filename  
    file = open(path, 'w')
    basename = os.path.splitext(filename)[0]
    
    file.write('#include \"' + basename + '.h\"\n\n')
    file.close()

def createHeaderFile(filename, classname = None, parentname = None):
    path = THIS_DIR + '/' + HEADERS_DIR + '/' + filename  
    file = open(path, 'w')
    basename = os.path.splitext(filename)[0]

    file.write('#pragma once\n\n')
    if classname != None:
        file.write('class ' + classname)
        if parentname != None:
            file.write(' : public ' + parentname);
        file.write('{\n\n};\n')

    file.close()

if len(sys.argv) >= 2:
    command = sys.argv[1]
    if command == "PJNewClass":
        if len(sys.argv) <= 3:
            newClass(sys.argv[2])
        elif len(sys.argv) >= 4:
            newClass(sys.argv[2], sys.argv[3])
    if command == "PJNewSource":
        if len(sys.argv) >= 3:
            newSource(sys.argv[2])
    if command == "PJNewHeader":
        if len(sys.argv) >= 3:
            newHeader(sys.argv[2])
    if command == "PJRemoveSource":
        if len(sys.argv) >= 3:
            removeSource(sys.argv[2])




