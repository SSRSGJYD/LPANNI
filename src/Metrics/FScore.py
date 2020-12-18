#!/usr/bin/env python
# coding: utf-8

# In[1]:


import time
import json


# In[2]:


def fetchNetWorkOverlappingNodes(filePath):
    nodeSet = set({})
    nodeMap = {}
    with open(filePath, 'r') as fileHandle:
        line = fileHandle.readline()
        while line:
            nodeIdList = line.split()
            for nodeId in nodeIdList:
                if nodeMap.get(nodeId):
                    nodeMap[nodeId] += 1
                else:
                    nodeMap[nodeId] = 1
            line = fileHandle.readline()
        for nodeId, count in nodeMap.items():
            if count > 1:
                nodeSet.add(nodeId)
    return nodeSet


# In[3]:


def fetchResultOverlappingNodes(filePath):
    nodeSet = set({})
    with open(filePath, 'r') as fileHandle:
        line = fileHandle.readline()
        while line:
            nodeIdList = line.split()
            if len(nodeIdList) > 2:
                nodeSet.add(nodeIdList[0])
            line = fileHandle.readline()
    return nodeSet


# In[4]:


def calculateFScore(communityFilePath, resultFilePath):
    groundTruth = fetchResultOverlappingNodes(communityFilePath)
    predictNodes = fetchResultOverlappingNodes(resultFilePath)
    correctNodes = predictNodes.intersection(groundTruth)
    precision = len(correctNodes)*1.0/len(predictNodes)
    recall = len(correctNodes)*1.0/len(groundTruth)
    if precision+recall > 0:
        fScore = 2*precision*recall/(precision+recall)
    else:
        fScore = 0
    #print('Precision: %.5f' % precision)
    #print('Recall: %.5f' % recall)
    #print('F1 Score: %.5f' % f1Score)
    return fScore, precision, recall


# In[5]:


if __name__ == '__main__':
    startTime = time.time()
    #communityFilePath = "../../data/Amazon/amazon.top5000.cmty.txt"
    #resultFilePath = "../../result/amazon_result.txt"
    #fScore, precision, recall = calculateFScore(communityFilePath, resultFilePath)
    with open('FScore.json', 'w') as fileHandle:
        #fileHandle.write('Amazon:\n\tFScore: %f\n\tPrecision: %f\n\tRecall: %f\n' % (fScore, precision, recall))
        jsonObj = {}
        for i in [1000, 5000, 10000]:
            jsonObj[str(i)] = {}
            for j in [1, 3]:
                jsonObj[str(i)]['0.%d' % j] = {'FScore': [], 'Precision': [], 'Recall': []}
                for k in [2, 3, 4, 5, 6, 7, 8]:
                    resultFilePath = "../../data/LFR/N-%d-mu0.%d-om%d/result.dat" % (i, j, k)
                    communityFilePath = "../../data/LFR/N-%d-mu0.%d-om%d/community.dat" % (i, j, k)
                    fScore, precision, recall = calculateFScore(communityFilePath, resultFilePath)
                    #fileHandle.write('N-%d-mu0.%d-om%d:\n\tFScore: %f\n\tPrecision: %f\n\tRecall: %f\n' % (i, j, k, fScore, precision, recall))
                    jsonObj[str(i)]['0.%d' % j]['FScore'].append(fScore)
                    jsonObj[str(i)]['0.%d' % j]['Precision'].append(precision)
                    jsonObj[str(i)]['0.%d' % j]['Recall'].append(recall)
        fileHandle.write(json.dumps(jsonObj))
    completeTime = time.time()
    print('Result Written in FScore.txt')
    print('Running Time: %.3fs' % (completeTime-startTime))


# In[ ]:




