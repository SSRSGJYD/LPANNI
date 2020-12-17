#!/usr/bin/env python
# coding: utf-8

# In[ ]:


from neo4j import GraphDatabase
import time
import json


# In[ ]:


class neo4jWriter:
    def __init__(self, uri, user, password):
        self.driver = GraphDatabase.driver(uri, auth=(user, password))
        return

    ### Result Writer ###
    def resultWriter(self, filePath):
        with self.driver.session() as session:
            with open(filePath, 'r') as fileHandle:
                # go through and create all Node
                batchStr = '['
                batchCount = 0
                totalCount = 0
                line = fileHandle.readline()
                while line:
                    batchStr = batchStr + self.__resultLineToJSON(line) +', '
                    batchCount += 1
                    if batchCount == 1000:
                        batchStr = batchStr[:-2] + ']'
                        session.write_transaction(self.__createResultBatchTx, batchStr)
                        totalCount += batchCount
                        batchCount = 0
                        batchStr = '['
                        print('Result: Write a batch. %s nodes completed.' % totalCount)
                    line = fileHandle.readline()
                if batchCount > 0:
                    batchStr = batchStr[:-2] + ']'
                    session.write_transaction(self.__createResultBatchTx, batchStr)
                    totalCount += batchCount
                    print('Result: Write the last batch. %s nodes completed.' % totalCount)
                # create CenterAt Edges
                session.write_transaction(self.__createAllCNTx)
                print('Result: All Done')
        return totalCount
    def __resultLineToJSON(self, line):
        # {nodeId: $nid, commList: [$cid, $cid, ...]}
        items = line.split()
        jsonStr = '{nodeId: %s, commList: [' % items[0]
        for item in items[1:]:
            jsonStr = jsonStr + '{commId: %s}, ' % item
        jsonStr = jsonStr[:-2] + ']}'
        return jsonStr
    def __createResultBatchTx(self, tx, batch):
        cypher = 'UNWIND %s AS item MERGE (n:Node {nid:item.nodeId}) FOREACH (comm IN item.commList |                   MERGE (c:Community {cid:comm.commId}) CREATE (n)-[:BelongTo]->(c))' % batch
        result = tx.run(cypher)
        return result.consume()
    def __createAllCNTx(self, tx):
        cypher = 'MATCH (c:Community), (n:Node) WHERE c.cid=n.nid CREATE (c)-[:CenterAt]->(n)'
        result = tx.run(cypher)
        return result.consume()

    ### Network Writer ###
    def networkWriter(self, filePath, bidir):
        with self.driver.session() as session:
            with open(filePath, 'r') as fileHandle:
                print("Network: %s nodes and %s edges in total." % (fileHandle.readline()[:-1], fileHandle.readline()[:-1]))
                # go through and create all Node
                batchStr = '['
                batchCount = 0
                totalCount = 0
                line = fileHandle.readline()
                while line:
                    batchStr = batchStr + self.__networkLineToJSON(line) +', '
                    batchCount += 1
                    if batchCount == 2000:
                        batchStr = batchStr[:-2] + ']'
                        session.write_transaction(self.__createNetworkBatchTx, batchStr, bidir)
                        totalCount += batchCount
                        batchCount = 0
                        batchStr = '['
                        print('Network: Write a batch. %s edges completed.' % totalCount)
                    line = fileHandle.readline()
                if batchCount > 0:
                    batchStr = batchStr[:-2] + ']'
                    session.write_transaction(self.__createNetworkBatchTx, batchStr, bidir)
                    totalCount += batchCount
                    print('Network: Write the last batch. %s edges completed.' % totalCount)
                print('Network: All Done')
        return totalCount
    def __networkLineToJSON(self, line):
        # {nodeId1: $nid1, nodeId2: $nid2}
        nodeId1, nodeId2 = line.split()
        jsonStr = '{nodeId1: %s, nodeId2: %s}' % (nodeId1, nodeId2)
        return jsonStr
    def __createNetworkBatchTx(self, tx, batch, bidir):
        cypher = 'UNWIND %s AS item MERGE (n1:Node {nid:item.nodeId1})                   MERGE (n2:Node {nid:item.nodeId2}) CREATE (n1)-[:Relation]->(n2)'
        if bidir:
            cypher = cypher + ', (n2)-[:Relation]->(n1)'
        result = tx.run(cypher % batch)
        return result.consume()

    ### Index Builder ###
    def indexBuilder(self):
        with self.driver.session() as session:
            session.write_transaction(self.__createIndexTx)
            print('Index: All Done')
        return
    def __createIndexTx(self, tx):
        # automatically create index when creating a unique constraint
        cypher = 'CREATE CONSTRAINT ON (n:Node) ASSERT n.nid IS UNIQUE'
        result = tx.run(cypher)
        result.consume()
        #cypher = 'CREATE INDEX ON :Node(nid)'
        #result = tx.run(cypher)
        #result.consume()
        
        # automatically create index when creating a unique constraint
        cypher = 'CREATE CONSTRAINT ON (c:Community) ASSERT c.cid IS UNIQUE'
        result = tx.run(cypher)
        result.consume()
        #cypher = 'CREATE INDEX ON :Community(cid)'
        #result = tx.run(cypher)
        #result.consume()
        return True

    ### Other Transactions(Abandoned) ###
    def __createNodeTx(self, tx, nodeId, nodeLabel, **nodeAttrs):
        cypher = 'CREATE (n: %s {nid: %s' % (nodeLabel, nodeId)
        if nodeAttrs:
            for item in nodeAttrs.items():
                cypher = cypher + ', %s: %s' % (item[0], item[1])
        cypher = cypher + '}) RETURN n'
        result = tx.run(cypher)
        return result.single()
    def __createRelTx(self, tx, nodeId1, nodeLabel1, nodeId2, nodeLabel2, relType, **relAttrs):
        cypher = 'MATCH (a:%s), (b:%s) WHERE a.nid = %s AND b.nid = %s CREATE (a)-[r: %s' % (nodeLabel1, nodeLabel2, nodeId1, nodeId2, relType)
        if relAttrs:
            cypher = cypher + ' {'
            for item in relAttrs.items():
                cypher = cypher + '%s: %s, ' % (item[0], item[1])
            cypher = cypher[:-2] + '}'
        cypher = cypher + ']->(b) RETURN r'
        result = tx.run(cypher)
        return result.single()
    def __matchNodeTx(self, tx, nodeId=None, nodeLabel=None, **nodeAttrs):
        cypher = 'match'
        if nodeLabel:
            cypher = cypher + ' (n:%s)' % nodeLabel
        else:
            cypher = cypher + ' (n)'
        if nodeId:
            cypher = cypher + ' where n.nid = %s' % nodeId
            if nodeAttrs:
                for item in nodeAttrs.items():
                    cypher = cypher + ' and %s = %s' % (item[0], item[1])
        else:
            if nodeAttrs:
                cypher = cypher + ' where' % nodeId
                for item in nodeAttrs.items():
                    cypher = cypher + ' %s = %s and' % (item[0], item[1])
                cypher = cypher[:-4]
        cypher = cypher + ' return n'
        result = tx.run(cypher)
        records = []
        for record in result:
            records.append(record['n'])
        return records


# In[3]:


if __name__ == '__main__':
    startTime = time.time()
    with open('settings.json', 'r') as fileHandle:
        settings = json.load(fileHandle)
        uri = settings['Neo4j']['uri']
        user = settings['Neo4j']['user']
        password = settings['Neo4j']['password']
        networkFilePath = settings['NetworkFilePath']
        resultFilePath = settings['ResultFilePath']
    writer = neo4jWriter(uri, user, password)
    writer.indexBuilder()
    writer.networkWriter(networkFilePath, True)
    writer.resultWriter(resultFilePath)
    completeTime = time.time()
    print('Running Time: %.3fs' % (completeTime-startTime))
    


# In[ ]:




