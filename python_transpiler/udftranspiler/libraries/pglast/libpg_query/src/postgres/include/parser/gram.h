/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     UIDENT = 259,
     FCONST = 260,
     SCONST = 261,
     USCONST = 262,
     BCONST = 263,
     XCONST = 264,
     Op = 265,
     ICONST = 266,
     PARAM = 267,
     TYPECAST = 268,
     DOT_DOT = 269,
     COLON_EQUALS = 270,
     EQUALS_GREATER = 271,
     LESS_EQUALS = 272,
     GREATER_EQUALS = 273,
     NOT_EQUALS = 274,
     SQL_COMMENT = 275,
     C_COMMENT = 276,
     ABORT_P = 277,
     ABSOLUTE_P = 278,
     ACCESS = 279,
     ACTION = 280,
     ADD_P = 281,
     ADMIN = 282,
     AFTER = 283,
     AGGREGATE = 284,
     ALL = 285,
     ALSO = 286,
     ALTER = 287,
     ALWAYS = 288,
     ANALYSE = 289,
     ANALYZE = 290,
     AND = 291,
     ANY = 292,
     ARRAY = 293,
     AS = 294,
     ASC = 295,
     ASENSITIVE = 296,
     ASSERTION = 297,
     ASSIGNMENT = 298,
     ASYMMETRIC = 299,
     ATOMIC = 300,
     AT = 301,
     ATTACH = 302,
     ATTRIBUTE = 303,
     AUTHORIZATION = 304,
     BACKWARD = 305,
     BEFORE = 306,
     BEGIN_P = 307,
     BETWEEN = 308,
     BIGINT = 309,
     BINARY = 310,
     BIT = 311,
     BOOLEAN_P = 312,
     BOTH = 313,
     BREADTH = 314,
     BY = 315,
     CACHE = 316,
     CALL = 317,
     CALLED = 318,
     CASCADE = 319,
     CASCADED = 320,
     CASE = 321,
     CAST = 322,
     CATALOG_P = 323,
     CHAIN = 324,
     CHAR_P = 325,
     CHARACTER = 326,
     CHARACTERISTICS = 327,
     CHECK = 328,
     CHECKPOINT = 329,
     CLASS = 330,
     CLOSE = 331,
     CLUSTER = 332,
     COALESCE = 333,
     COLLATE = 334,
     COLLATION = 335,
     COLUMN = 336,
     COLUMNS = 337,
     COMMENT = 338,
     COMMENTS = 339,
     COMMIT = 340,
     COMMITTED = 341,
     COMPRESSION = 342,
     CONCURRENTLY = 343,
     CONFIGURATION = 344,
     CONFLICT = 345,
     CONNECTION = 346,
     CONSTRAINT = 347,
     CONSTRAINTS = 348,
     CONTENT_P = 349,
     CONTINUE_P = 350,
     CONVERSION_P = 351,
     COPY = 352,
     COST = 353,
     CREATE = 354,
     CROSS = 355,
     CSV = 356,
     CUBE = 357,
     CURRENT_P = 358,
     CURRENT_CATALOG = 359,
     CURRENT_DATE = 360,
     CURRENT_ROLE = 361,
     CURRENT_SCHEMA = 362,
     CURRENT_TIME = 363,
     CURRENT_TIMESTAMP = 364,
     CURRENT_USER = 365,
     CURSOR = 366,
     CYCLE = 367,
     DATA_P = 368,
     DATABASE = 369,
     DAY_P = 370,
     DEALLOCATE = 371,
     DEC = 372,
     DECIMAL_P = 373,
     DECLARE = 374,
     DEFAULT = 375,
     DEFAULTS = 376,
     DEFERRABLE = 377,
     DEFERRED = 378,
     DEFINER = 379,
     DELETE_P = 380,
     DELIMITER = 381,
     DELIMITERS = 382,
     DEPENDS = 383,
     DEPTH = 384,
     DESC = 385,
     DETACH = 386,
     DICTIONARY = 387,
     DISABLE_P = 388,
     DISCARD = 389,
     DISTINCT = 390,
     DO = 391,
     DOCUMENT_P = 392,
     DOMAIN_P = 393,
     DOUBLE_P = 394,
     DROP = 395,
     EACH = 396,
     ELSE = 397,
     ENABLE_P = 398,
     ENCODING = 399,
     ENCRYPTED = 400,
     END_P = 401,
     ENUM_P = 402,
     ESCAPE = 403,
     EVENT = 404,
     EXCEPT = 405,
     EXCLUDE = 406,
     EXCLUDING = 407,
     EXCLUSIVE = 408,
     EXECUTE = 409,
     EXISTS = 410,
     EXPLAIN = 411,
     EXPRESSION = 412,
     EXTENSION = 413,
     EXTERNAL = 414,
     EXTRACT = 415,
     FALSE_P = 416,
     FAMILY = 417,
     FETCH = 418,
     FILTER = 419,
     FINALIZE = 420,
     FIRST_P = 421,
     FLOAT_P = 422,
     FOLLOWING = 423,
     FOR = 424,
     FORCE = 425,
     FOREIGN = 426,
     FORWARD = 427,
     FREEZE = 428,
     FROM = 429,
     FULL = 430,
     FUNCTION = 431,
     FUNCTIONS = 432,
     GENERATED = 433,
     GLOBAL = 434,
     GRANT = 435,
     GRANTED = 436,
     GREATEST = 437,
     GROUP_P = 438,
     GROUPING = 439,
     GROUPS = 440,
     HANDLER = 441,
     HAVING = 442,
     HEADER_P = 443,
     HOLD = 444,
     HOUR_P = 445,
     IDENTITY_P = 446,
     IF_P = 447,
     ILIKE = 448,
     IMMEDIATE = 449,
     IMMUTABLE = 450,
     IMPLICIT_P = 451,
     IMPORT_P = 452,
     IN_P = 453,
     INCLUDE = 454,
     INCLUDING = 455,
     INCREMENT = 456,
     INDEX = 457,
     INDEXES = 458,
     INHERIT = 459,
     INHERITS = 460,
     INITIALLY = 461,
     INLINE_P = 462,
     INNER_P = 463,
     INOUT = 464,
     INPUT_P = 465,
     INSENSITIVE = 466,
     INSERT = 467,
     INSTEAD = 468,
     INT_P = 469,
     INTEGER = 470,
     INTERSECT = 471,
     INTERVAL = 472,
     INTO = 473,
     INVOKER = 474,
     IS = 475,
     ISNULL = 476,
     ISOLATION = 477,
     JOIN = 478,
     KEY = 479,
     LABEL = 480,
     LANGUAGE = 481,
     LARGE_P = 482,
     LAST_P = 483,
     LATERAL_P = 484,
     LEADING = 485,
     LEAKPROOF = 486,
     LEAST = 487,
     LEFT = 488,
     LEVEL = 489,
     LIKE = 490,
     LIMIT = 491,
     LISTEN = 492,
     LOAD = 493,
     LOCAL = 494,
     LOCALTIME = 495,
     LOCALTIMESTAMP = 496,
     LOCATION = 497,
     LOCK_P = 498,
     LOCKED = 499,
     LOGGED = 500,
     MAPPING = 501,
     MATCH = 502,
     MATERIALIZED = 503,
     MAXVALUE = 504,
     METHOD = 505,
     MINUTE_P = 506,
     MINVALUE = 507,
     MODE = 508,
     MONTH_P = 509,
     MOVE = 510,
     NAME_P = 511,
     NAMES = 512,
     NATIONAL = 513,
     NATURAL = 514,
     NCHAR = 515,
     NEW = 516,
     NEXT = 517,
     NFC = 518,
     NFD = 519,
     NFKC = 520,
     NFKD = 521,
     NO = 522,
     NONE = 523,
     NORMALIZE = 524,
     NORMALIZED = 525,
     NOT = 526,
     NOTHING = 527,
     NOTIFY = 528,
     NOTNULL = 529,
     NOWAIT = 530,
     NULL_P = 531,
     NULLIF = 532,
     NULLS_P = 533,
     NUMERIC = 534,
     OBJECT_P = 535,
     OF = 536,
     OFF = 537,
     OFFSET = 538,
     OIDS = 539,
     OLD = 540,
     ON = 541,
     ONLY = 542,
     OPERATOR = 543,
     OPTION = 544,
     OPTIONS = 545,
     OR = 546,
     ORDER = 547,
     ORDINALITY = 548,
     OTHERS = 549,
     OUT_P = 550,
     OUTER_P = 551,
     OVER = 552,
     OVERLAPS = 553,
     OVERLAY = 554,
     OVERRIDING = 555,
     OWNED = 556,
     OWNER = 557,
     PARALLEL = 558,
     PARSER = 559,
     PARTIAL = 560,
     PARTITION = 561,
     PASSING = 562,
     PASSWORD = 563,
     PLACING = 564,
     PLANS = 565,
     POLICY = 566,
     POSITION = 567,
     PRECEDING = 568,
     PRECISION = 569,
     PRESERVE = 570,
     PREPARE = 571,
     PREPARED = 572,
     PRIMARY = 573,
     PRIOR = 574,
     PRIVILEGES = 575,
     PROCEDURAL = 576,
     PROCEDURE = 577,
     PROCEDURES = 578,
     PROGRAM = 579,
     PUBLICATION = 580,
     QUOTE = 581,
     RANGE = 582,
     READ = 583,
     REAL = 584,
     REASSIGN = 585,
     RECHECK = 586,
     RECURSIVE = 587,
     REF_P = 588,
     REFERENCES = 589,
     REFERENCING = 590,
     REFRESH = 591,
     REINDEX = 592,
     RELATIVE_P = 593,
     RELEASE = 594,
     RENAME = 595,
     REPEATABLE = 596,
     REPLACE = 597,
     REPLICA = 598,
     RESET = 599,
     RESTART = 600,
     RESTRICT = 601,
     RETURN = 602,
     RETURNING = 603,
     RETURNS = 604,
     REVOKE = 605,
     RIGHT = 606,
     ROLE = 607,
     ROLLBACK = 608,
     ROLLUP = 609,
     ROUTINE = 610,
     ROUTINES = 611,
     ROW = 612,
     ROWS = 613,
     RULE = 614,
     SAVEPOINT = 615,
     SCHEMA = 616,
     SCHEMAS = 617,
     SCROLL = 618,
     SEARCH = 619,
     SECOND_P = 620,
     SECURITY = 621,
     SELECT = 622,
     SEQUENCE = 623,
     SEQUENCES = 624,
     SERIALIZABLE = 625,
     SERVER = 626,
     SESSION = 627,
     SESSION_USER = 628,
     SET = 629,
     SETS = 630,
     SETOF = 631,
     SHARE = 632,
     SHOW = 633,
     SIMILAR = 634,
     SIMPLE = 635,
     SKIP = 636,
     SMALLINT = 637,
     SNAPSHOT = 638,
     SOME = 639,
     SQL_P = 640,
     STABLE = 641,
     STANDALONE_P = 642,
     START = 643,
     STATEMENT = 644,
     STATISTICS = 645,
     STDIN = 646,
     STDOUT = 647,
     STORAGE = 648,
     STORED = 649,
     STRICT_P = 650,
     STRIP_P = 651,
     SUBSCRIPTION = 652,
     SUBSTRING = 653,
     SUPPORT = 654,
     SYMMETRIC = 655,
     SYSID = 656,
     SYSTEM_P = 657,
     TABLE = 658,
     TABLES = 659,
     TABLESAMPLE = 660,
     TABLESPACE = 661,
     TEMP = 662,
     TEMPLATE = 663,
     TEMPORARY = 664,
     TEXT_P = 665,
     THEN = 666,
     TIES = 667,
     TIME = 668,
     TIMESTAMP = 669,
     TO = 670,
     TRAILING = 671,
     TRANSACTION = 672,
     TRANSFORM = 673,
     TREAT = 674,
     TRIGGER = 675,
     TRIM = 676,
     TRUE_P = 677,
     TRUNCATE = 678,
     TRUSTED = 679,
     TYPE_P = 680,
     TYPES_P = 681,
     UESCAPE = 682,
     UNBOUNDED = 683,
     UNCOMMITTED = 684,
     UNENCRYPTED = 685,
     UNION = 686,
     UNIQUE = 687,
     UNKNOWN = 688,
     UNLISTEN = 689,
     UNLOGGED = 690,
     UNTIL = 691,
     UPDATE = 692,
     USER = 693,
     USING = 694,
     VACUUM = 695,
     VALID = 696,
     VALIDATE = 697,
     VALIDATOR = 698,
     VALUE_P = 699,
     VALUES = 700,
     VARCHAR = 701,
     VARIADIC = 702,
     VARYING = 703,
     VERBOSE = 704,
     VERSION_P = 705,
     VIEW = 706,
     VIEWS = 707,
     VOLATILE = 708,
     WHEN = 709,
     WHERE = 710,
     WHITESPACE_P = 711,
     WINDOW = 712,
     WITH = 713,
     WITHIN = 714,
     WITHOUT = 715,
     WORK = 716,
     WRAPPER = 717,
     WRITE = 718,
     XML_P = 719,
     XMLATTRIBUTES = 720,
     XMLCONCAT = 721,
     XMLELEMENT = 722,
     XMLEXISTS = 723,
     XMLFOREST = 724,
     XMLNAMESPACES = 725,
     XMLPARSE = 726,
     XMLPI = 727,
     XMLROOT = 728,
     XMLSERIALIZE = 729,
     XMLTABLE = 730,
     YEAR_P = 731,
     YES_P = 732,
     ZONE = 733,
     NOT_LA = 734,
     NULLS_LA = 735,
     WITH_LA = 736,
     MODE_TYPE_NAME = 737,
     MODE_PLPGSQL_EXPR = 738,
     MODE_PLPGSQL_ASSIGN1 = 739,
     MODE_PLPGSQL_ASSIGN2 = 740,
     MODE_PLPGSQL_ASSIGN3 = 741,
     UMINUS = 742
   };
#endif
/* Tokens.  */
#define IDENT 258
#define UIDENT 259
#define FCONST 260
#define SCONST 261
#define USCONST 262
#define BCONST 263
#define XCONST 264
#define Op 265
#define ICONST 266
#define PARAM 267
#define TYPECAST 268
#define DOT_DOT 269
#define COLON_EQUALS 270
#define EQUALS_GREATER 271
#define LESS_EQUALS 272
#define GREATER_EQUALS 273
#define NOT_EQUALS 274
#define SQL_COMMENT 275
#define C_COMMENT 276
#define ABORT_P 277
#define ABSOLUTE_P 278
#define ACCESS 279
#define ACTION 280
#define ADD_P 281
#define ADMIN 282
#define AFTER 283
#define AGGREGATE 284
#define ALL 285
#define ALSO 286
#define ALTER 287
#define ALWAYS 288
#define ANALYSE 289
#define ANALYZE 290
#define AND 291
#define ANY 292
#define ARRAY 293
#define AS 294
#define ASC 295
#define ASENSITIVE 296
#define ASSERTION 297
#define ASSIGNMENT 298
#define ASYMMETRIC 299
#define ATOMIC 300
#define AT 301
#define ATTACH 302
#define ATTRIBUTE 303
#define AUTHORIZATION 304
#define BACKWARD 305
#define BEFORE 306
#define BEGIN_P 307
#define BETWEEN 308
#define BIGINT 309
#define BINARY 310
#define BIT 311
#define BOOLEAN_P 312
#define BOTH 313
#define BREADTH 314
#define BY 315
#define CACHE 316
#define CALL 317
#define CALLED 318
#define CASCADE 319
#define CASCADED 320
#define CASE 321
#define CAST 322
#define CATALOG_P 323
#define CHAIN 324
#define CHAR_P 325
#define CHARACTER 326
#define CHARACTERISTICS 327
#define CHECK 328
#define CHECKPOINT 329
#define CLASS 330
#define CLOSE 331
#define CLUSTER 332
#define COALESCE 333
#define COLLATE 334
#define COLLATION 335
#define COLUMN 336
#define COLUMNS 337
#define COMMENT 338
#define COMMENTS 339
#define COMMIT 340
#define COMMITTED 341
#define COMPRESSION 342
#define CONCURRENTLY 343
#define CONFIGURATION 344
#define CONFLICT 345
#define CONNECTION 346
#define CONSTRAINT 347
#define CONSTRAINTS 348
#define CONTENT_P 349
#define CONTINUE_P 350
#define CONVERSION_P 351
#define COPY 352
#define COST 353
#define CREATE 354
#define CROSS 355
#define CSV 356
#define CUBE 357
#define CURRENT_P 358
#define CURRENT_CATALOG 359
#define CURRENT_DATE 360
#define CURRENT_ROLE 361
#define CURRENT_SCHEMA 362
#define CURRENT_TIME 363
#define CURRENT_TIMESTAMP 364
#define CURRENT_USER 365
#define CURSOR 366
#define CYCLE 367
#define DATA_P 368
#define DATABASE 369
#define DAY_P 370
#define DEALLOCATE 371
#define DEC 372
#define DECIMAL_P 373
#define DECLARE 374
#define DEFAULT 375
#define DEFAULTS 376
#define DEFERRABLE 377
#define DEFERRED 378
#define DEFINER 379
#define DELETE_P 380
#define DELIMITER 381
#define DELIMITERS 382
#define DEPENDS 383
#define DEPTH 384
#define DESC 385
#define DETACH 386
#define DICTIONARY 387
#define DISABLE_P 388
#define DISCARD 389
#define DISTINCT 390
#define DO 391
#define DOCUMENT_P 392
#define DOMAIN_P 393
#define DOUBLE_P 394
#define DROP 395
#define EACH 396
#define ELSE 397
#define ENABLE_P 398
#define ENCODING 399
#define ENCRYPTED 400
#define END_P 401
#define ENUM_P 402
#define ESCAPE 403
#define EVENT 404
#define EXCEPT 405
#define EXCLUDE 406
#define EXCLUDING 407
#define EXCLUSIVE 408
#define EXECUTE 409
#define EXISTS 410
#define EXPLAIN 411
#define EXPRESSION 412
#define EXTENSION 413
#define EXTERNAL 414
#define EXTRACT 415
#define FALSE_P 416
#define FAMILY 417
#define FETCH 418
#define FILTER 419
#define FINALIZE 420
#define FIRST_P 421
#define FLOAT_P 422
#define FOLLOWING 423
#define FOR 424
#define FORCE 425
#define FOREIGN 426
#define FORWARD 427
#define FREEZE 428
#define FROM 429
#define FULL 430
#define FUNCTION 431
#define FUNCTIONS 432
#define GENERATED 433
#define GLOBAL 434
#define GRANT 435
#define GRANTED 436
#define GREATEST 437
#define GROUP_P 438
#define GROUPING 439
#define GROUPS 440
#define HANDLER 441
#define HAVING 442
#define HEADER_P 443
#define HOLD 444
#define HOUR_P 445
#define IDENTITY_P 446
#define IF_P 447
#define ILIKE 448
#define IMMEDIATE 449
#define IMMUTABLE 450
#define IMPLICIT_P 451
#define IMPORT_P 452
#define IN_P 453
#define INCLUDE 454
#define INCLUDING 455
#define INCREMENT 456
#define INDEX 457
#define INDEXES 458
#define INHERIT 459
#define INHERITS 460
#define INITIALLY 461
#define INLINE_P 462
#define INNER_P 463
#define INOUT 464
#define INPUT_P 465
#define INSENSITIVE 466
#define INSERT 467
#define INSTEAD 468
#define INT_P 469
#define INTEGER 470
#define INTERSECT 471
#define INTERVAL 472
#define INTO 473
#define INVOKER 474
#define IS 475
#define ISNULL 476
#define ISOLATION 477
#define JOIN 478
#define KEY 479
#define LABEL 480
#define LANGUAGE 481
#define LARGE_P 482
#define LAST_P 483
#define LATERAL_P 484
#define LEADING 485
#define LEAKPROOF 486
#define LEAST 487
#define LEFT 488
#define LEVEL 489
#define LIKE 490
#define LIMIT 491
#define LISTEN 492
#define LOAD 493
#define LOCAL 494
#define LOCALTIME 495
#define LOCALTIMESTAMP 496
#define LOCATION 497
#define LOCK_P 498
#define LOCKED 499
#define LOGGED 500
#define MAPPING 501
#define MATCH 502
#define MATERIALIZED 503
#define MAXVALUE 504
#define METHOD 505
#define MINUTE_P 506
#define MINVALUE 507
#define MODE 508
#define MONTH_P 509
#define MOVE 510
#define NAME_P 511
#define NAMES 512
#define NATIONAL 513
#define NATURAL 514
#define NCHAR 515
#define NEW 516
#define NEXT 517
#define NFC 518
#define NFD 519
#define NFKC 520
#define NFKD 521
#define NO 522
#define NONE 523
#define NORMALIZE 524
#define NORMALIZED 525
#define NOT 526
#define NOTHING 527
#define NOTIFY 528
#define NOTNULL 529
#define NOWAIT 530
#define NULL_P 531
#define NULLIF 532
#define NULLS_P 533
#define NUMERIC 534
#define OBJECT_P 535
#define OF 536
#define OFF 537
#define OFFSET 538
#define OIDS 539
#define OLD 540
#define ON 541
#define ONLY 542
#define OPERATOR 543
#define OPTION 544
#define OPTIONS 545
#define OR 546
#define ORDER 547
#define ORDINALITY 548
#define OTHERS 549
#define OUT_P 550
#define OUTER_P 551
#define OVER 552
#define OVERLAPS 553
#define OVERLAY 554
#define OVERRIDING 555
#define OWNED 556
#define OWNER 557
#define PARALLEL 558
#define PARSER 559
#define PARTIAL 560
#define PARTITION 561
#define PASSING 562
#define PASSWORD 563
#define PLACING 564
#define PLANS 565
#define POLICY 566
#define POSITION 567
#define PRECEDING 568
#define PRECISION 569
#define PRESERVE 570
#define PREPARE 571
#define PREPARED 572
#define PRIMARY 573
#define PRIOR 574
#define PRIVILEGES 575
#define PROCEDURAL 576
#define PROCEDURE 577
#define PROCEDURES 578
#define PROGRAM 579
#define PUBLICATION 580
#define QUOTE 581
#define RANGE 582
#define READ 583
#define REAL 584
#define REASSIGN 585
#define RECHECK 586
#define RECURSIVE 587
#define REF_P 588
#define REFERENCES 589
#define REFERENCING 590
#define REFRESH 591
#define REINDEX 592
#define RELATIVE_P 593
#define RELEASE 594
#define RENAME 595
#define REPEATABLE 596
#define REPLACE 597
#define REPLICA 598
#define RESET 599
#define RESTART 600
#define RESTRICT 601
#define RETURN 602
#define RETURNING 603
#define RETURNS 604
#define REVOKE 605
#define RIGHT 606
#define ROLE 607
#define ROLLBACK 608
#define ROLLUP 609
#define ROUTINE 610
#define ROUTINES 611
#define ROW 612
#define ROWS 613
#define RULE 614
#define SAVEPOINT 615
#define SCHEMA 616
#define SCHEMAS 617
#define SCROLL 618
#define SEARCH 619
#define SECOND_P 620
#define SECURITY 621
#define SELECT 622
#define SEQUENCE 623
#define SEQUENCES 624
#define SERIALIZABLE 625
#define SERVER 626
#define SESSION 627
#define SESSION_USER 628
#define SET 629
#define SETS 630
#define SETOF 631
#define SHARE 632
#define SHOW 633
#define SIMILAR 634
#define SIMPLE 635
#define SKIP 636
#define SMALLINT 637
#define SNAPSHOT 638
#define SOME 639
#define SQL_P 640
#define STABLE 641
#define STANDALONE_P 642
#define START 643
#define STATEMENT 644
#define STATISTICS 645
#define STDIN 646
#define STDOUT 647
#define STORAGE 648
#define STORED 649
#define STRICT_P 650
#define STRIP_P 651
#define SUBSCRIPTION 652
#define SUBSTRING 653
#define SUPPORT 654
#define SYMMETRIC 655
#define SYSID 656
#define SYSTEM_P 657
#define TABLE 658
#define TABLES 659
#define TABLESAMPLE 660
#define TABLESPACE 661
#define TEMP 662
#define TEMPLATE 663
#define TEMPORARY 664
#define TEXT_P 665
#define THEN 666
#define TIES 667
#define TIME 668
#define TIMESTAMP 669
#define TO 670
#define TRAILING 671
#define TRANSACTION 672
#define TRANSFORM 673
#define TREAT 674
#define TRIGGER 675
#define TRIM 676
#define TRUE_P 677
#define TRUNCATE 678
#define TRUSTED 679
#define TYPE_P 680
#define TYPES_P 681
#define UESCAPE 682
#define UNBOUNDED 683
#define UNCOMMITTED 684
#define UNENCRYPTED 685
#define UNION 686
#define UNIQUE 687
#define UNKNOWN 688
#define UNLISTEN 689
#define UNLOGGED 690
#define UNTIL 691
#define UPDATE 692
#define USER 693
#define USING 694
#define VACUUM 695
#define VALID 696
#define VALIDATE 697
#define VALIDATOR 698
#define VALUE_P 699
#define VALUES 700
#define VARCHAR 701
#define VARIADIC 702
#define VARYING 703
#define VERBOSE 704
#define VERSION_P 705
#define VIEW 706
#define VIEWS 707
#define VOLATILE 708
#define WHEN 709
#define WHERE 710
#define WHITESPACE_P 711
#define WINDOW 712
#define WITH 713
#define WITHIN 714
#define WITHOUT 715
#define WORK 716
#define WRAPPER 717
#define WRITE 718
#define XML_P 719
#define XMLATTRIBUTES 720
#define XMLCONCAT 721
#define XMLELEMENT 722
#define XMLEXISTS 723
#define XMLFOREST 724
#define XMLNAMESPACES 725
#define XMLPARSE 726
#define XMLPI 727
#define XMLROOT 728
#define XMLSERIALIZE 729
#define XMLTABLE 730
#define YEAR_P 731
#define YES_P 732
#define ZONE 733
#define NOT_LA 734
#define NULLS_LA 735
#define WITH_LA 736
#define MODE_TYPE_NAME 737
#define MODE_PLPGSQL_EXPR 738
#define MODE_PLPGSQL_ASSIGN1 739
#define MODE_PLPGSQL_ASSIGN2 740
#define MODE_PLPGSQL_ASSIGN3 741
#define UMINUS 742




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 219 "gram.y"
{
	core_YYSTYPE		core_yystype;
	/* these fields must match core_YYSTYPE: */
	int					ival;
	char				*str;
	const char			*keyword;

	char				chr;
	bool				boolean;
	JoinType			jtype;
	DropBehavior		dbehavior;
	OnCommitAction		oncommit;
	List				*list;
	Node				*node;
	Value				*value;
	ObjectType			objtype;
	TypeName			*typnam;
	FunctionParameter   *fun_param;
	FunctionParameterMode fun_param_mode;
	ObjectWithArgs		*objwithargs;
	DefElem				*defelt;
	SortBy				*sortby;
	WindowDef			*windef;
	JoinExpr			*jexpr;
	IndexElem			*ielem;
	StatsElem			*selem;
	Alias				*alias;
	RangeVar			*range;
	IntoClause			*into;
	WithClause			*with;
	InferClause			*infer;
	OnConflictClause	*onconflict;
	A_Indices			*aind;
	ResTarget			*target;
	struct PrivTarget	*privtarget;
	AccessPriv			*accesspriv;
	struct ImportQual	*importqual;
	InsertStmt			*istmt;
	VariableSetStmt		*vsetstmt;
	PartitionElem		*partelem;
	PartitionSpec		*partspec;
	PartitionBoundSpec	*partboundspec;
	RoleSpec			*rolespec;
	struct SelectLimit	*selectlimit;
	SetQuantifier	 setquantifier;
	struct GroupClause  *groupclause;
}
/* Line 1529 of yacc.c.  */
#line 1071 "gram.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

