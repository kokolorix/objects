--
-- File generated with SQLiteStudio v3.1.1 on So. Aug 20 12:31:26 2017
--
-- Text encoding used: System
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: hierarchy
CREATE TABLE hierarchy (
    id     INTEGER PRIMARY KEY
                   NOT NULL,
    object INTEGER DEFAULT (0) 
                   REFERENCES object (id) ON DELETE NO ACTION
                                          ON UPDATE NO ACTION
                                          MATCH SIMPLE
                   NOT NULL,
    parent INTEGER REFERENCES object (id) ON DELETE NO ACTION
                                          ON UPDATE NO ACTION
                                          MATCH SIMPLE
);


-- Table: object
CREATE TABLE object (
    id      INTEGER PRIMARY KEY
                    NOT NULL,
    name    STRING,
    type    INTEGER REFERENCES object (id) ON DELETE NO ACTION
                                           ON UPDATE NO ACTION
                                           MATCH SIMPLE,
    version INTEGER NOT NULL
                    DEFAULT (1) 
);

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       1,
                       'BaseType',
                       1,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       2,
                       'ValueType',
                       1,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       3,
                       'ObjectType',
                       1,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       4,
                       'StringValue',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       5,
                       'BooleanValue',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       6,
                       'Int32Value',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       7,
                       'UInt32Value',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       8,
                       'FloatValue',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       9,
                       'UuIdValue',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       10,
                       'ObjectValue',
                       2,
                       1
                   );

INSERT INTO object (
                       id,
                       name,
                       type,
                       version
                   )
                   VALUES (
                       11,
                       'VectorValue',
                       2,
                       1
                   );


-- Table: property
CREATE TABLE property (
    id      INTEGER PRIMARY KEY,
    name    STRING,
    type    INTEGER REFERENCES object (id) ON DELETE NO ACTION
                                           ON UPDATE NO ACTION
                                           MATCH SIMPLE
                    NOT NULL,
    value   STRING,
    object  INTEGER REFERENCES object (id) ON DELETE NO ACTION
                                           ON UPDATE NO ACTION
                                           MATCH SIMPLE
                    NOT NULL,
    version INTEGER NOT NULL
                    DEFAULT (1) 
);

INSERT INTO property (
                         id,
                         name,
                         type,
                         value,
                         object,
                         version
                     )
                     VALUES (
                         1,
                         'id',
                         9,
                         NULL,
                         3,
                         1
                     );


COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
