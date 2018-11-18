# minidata

minidata is a simple database. It has not a language, just commands. It will has 2 mode; **search** mode and **insert** mode. If database in search mode, inserted data will index automatically. Insert mode will be for rapid insertion.


# Commands

|Command    |Parameters             |Description                    	|
|-----------|-----------------------|---------------------------------|
|CREATE     |`name:string`          |Creates new database             |
|DATABASE		|`name:string`          |Goes to database context         |
|DROP       |`name:string`					|Drops database                   |
|SELECT			|`condition:expression` |Finds and goes to record context |
|UPDATE			|`data:string`					|Updates record						        |
|DELETE			|								        |Deletes record						        |
|MODE			  |`mode:[SEARCH, INSERT]`|Selects database mode				    |
|CONTEXT		|								        |Tells current context						|
|EXIT			  |								        |Closing							            |
|QUIT			  |								        |Closing							            |

## Compiling

> $make

## Context Map

Terminal > Database > Entity > Record
