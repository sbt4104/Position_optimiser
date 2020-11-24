from __future__ import division
import sys, os, re, csv
import matplotlib.pyplot as plt
from matplotlib import interactive
import argparse
from argparse import RawTextHelpFormatter
import math

#interactive(True)

def validateArguments(args):
    if(not args.blockfile):
        print("At least the file containing the block info should be passed in the command line")
        return False
    return True


## Function to read the block and port placement file and extract the co-ordinates and create a DS which plotter accepts
#
#  param filePath : Path to the block/port placement file
# param dataType : type of the data, it can be block/port

def readFileData(filePath, fileType=""):
    resultList = []
    shapeCenterDict = {}
    if(not os.path.exists(filePath)):
        print("invalid file path "+filePath)
        return resultList

    with open(filePath, "r") as csvfile:
        csvReader = csv.reader(csvfile, delimiter=",")

        rowNumber = 0
        for row in csvReader:
            rowNumber += 1 
            if(not row):
                continue
            if("block" in row or "coordiantes" in row):
                continue
            if(len(row) != 2):
                print("Expected only two values in the "+dataType+" file, but which is not the case at row "+str(rowNumber)+" of file "+filePath+", hece could not process this row")
                continue

            dataValid = True
            coordinateData = row[1].strip()
            if(coordinateData == ""):
                print("empty data found in the row "+str(rowNumber)+" of file "+filePath+", hence could not process this row")
                continue
            blockName = row[0]
            coordinatesLst = re.findall("{.*?}", coordinateData)
            if(not coordinatesLst):
                continue
            x = []
            y = []
            for coordinate in coordinatesLst:
                coordinate = (coordinate.strip().strip("{").strip("}")).replace("  ", " ")
                coordinateLst = coordinate.split(" ")
                if(len(coordinateLst) != 2):
                    print("invalid syntax/coordiantes mentioned in "+coordinate+", so the shape will be ignored")
                    dataValid = False
                    break
                x.append(int(coordinateLst[0]))
                y.append(int(coordinateLst[1]))
            if(dataValid):
                shapeCenterDict[blockName] = (sum(x)/len(x), sum(y)/len(y))
                x.append(x[0])
                y.append(y[0])
            resultList.append([x, y, blockName])
    return resultList, shapeCenterDict



## Function to read the connecivity file and connect the ports using the port center coordinate calculated while reading the port placement data
#
#  param filePath : Path to the connecivity file
#  param portCenterDict : Dict which contains the port name as key and tuple/list containing the x and y coordinate as value (x and y are coordinates of center of the port)
#                           ex : { "port_01" : (100, 100) }

def readConnectivityData(filePath, portCenterDict):
    connectionsIgnored = 0
    totalScore = 0
    resultList = []
    if(not os.path.exists(filePath)):
        print("invalid file path "+filePath)
        return resultList

    with open(filePath, "r") as csvfile:
        csvReader = csv.reader(csvfile, delimiter=",")
        connCount = 0
        rowNumber = 0
        for row in csvReader:
            rowNumber += 1 
            if(not row):
                continue
            if("srcPort" in row or "destPort" in row):
                continue
            if(len(row) != 4):
                print("Expected only four values in the connecitivy file, but which is not the case at row "+str(rowNumber)+" of file "+filePath+", hece could not process this row")
                continue

            dataValid = True
            srcPort = row[0].split(".")[1]
            destPort = row[1].split(".")[1]
            width = row[2].strip()
            freq = row[3].strip()
            if(srcPort not in portCenterDict):
                print("Src port "+srcPort+" not found in port placement file, hence connections related to this port will be ignored")
                connectionsIgnored += 1
                continue
            if(destPort not in portCenterDict):
                print("Dest port "+destPort+" not found in port placement file, hence connections related to this port  will be ignored")
                connectionsIgnored += 1
                continue
            resultList.append([[portCenterDict[srcPort][0], portCenterDict[destPort][0]], [portCenterDict[srcPort][1], portCenterDict[destPort][1]], "conn"+str(connCount)])
            connCount += 1
            try:
                freq = float(freq)
            except:
                print("Invalid frequency value "+freq+" found, expected only decimal values, hence score for the connection "+row[0]+"->"+row[1]+" will be ignored")
                connectionsIgnored += 1
                continue
            distance = math.sqrt(((portCenterDict[srcPort][0]-portCenterDict[destPort][0])**2)+((portCenterDict[srcPort][1]-portCenterDict[destPort][1])**2))
            score = (1000/float(freq)) - (0.5 * distance)
            #print(distance, score)
            totalScore += score
            print("Distance and Score of conn "+row[0]+"->"+row[1]+"  = "+str(distance)+"  "+str(score))
    print("\n\n"+"-"*20+"Result Summary"+"-"*20)
    print("Total number of connections".ljust(35)+"= "+str(connCount))
    print("Number of connections ignored".ljust(35)+"= "+str(connectionsIgnored))
    print("Total score".ljust(35)+"= "+str(totalScore))
    return resultList
 


## Function to feed the data to the plotter
#
#  param coordinateDataList :  List containing the coniguous x-cordinates, y-co-ordinates and the shape name
#  ex : [[[300, 300, 610, 610, 300], [350, 655, 655, 350, 350], 'abc_0'], [[300, 300, 600, 600, 300], [950, 1150, 1150, 950, 950], 'abc_1']]
#       corresponding co-ordiantes are
#           (300 350) (300 655) (610 655) (610 350) (300 350)  -> block_name - abc_0
#           (300 950) (300 1150) (600 1150) (600 950) (300 950) -> block_name - abc_1
# param dataType : type of the data, it can be block/port/connectivity

def plotGraph(coordinateDataList, dataType=""):
    plt.plot([0], [0])
    for data in coordinateDataList:
        if(dataType == "port"):
            plt.plot(data[0], data[1], label=data[2])
        else:
            plt.plot(data[0], data[1], label=data[2], marker='o')



## Function which calls the plotter to plot the result in the x and y axis based on the data fed before
#
def finalPlot():
    plt.legend(loc='upper left', bbox_to_anchor=(-0.25, 1),)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()


if(__name__ == "__main__"):
    parser = argparse.ArgumentParser(description="Read the csv(comma separated) files and plot the shapes in cartesian coordinate system in two dimensions.", \
             formatter_class=RawTextHelpFormatter)
    parser.add_argument('-blockfile',
                         help='file path of the shape csv file', required=True)
    parser.add_argument('-portfile',
                         help='file path of the port csv file')
    parser.add_argument('-connectivityfile',
                         help='file path of the connectivity csv file')
    args = parser.parse_args()
    
    if(not validateArguments(args)):
        sys.exit()
    blockResultList, resultDict = readFileData(args.blockfile, "block")
    if(blockResultList):
        plotGraph(blockResultList)
    if(args.portfile):
        portResultList, portCenterDict = readFileData(args.portfile, "port")
        plotGraph(portResultList, "port")
        if(portResultList and args.connectivityfile):
            connectivityResultList = readConnectivityData(args.connectivityfile, portCenterDict)
            plotGraph(connectivityResultList)
            pass
    finalPlot()
