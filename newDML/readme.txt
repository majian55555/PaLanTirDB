PaLanTir Key Words:
select,insert,delete,update,create,destroy,load,print,help,quit,into,where,from,set,table,values,database,open,close,int,real,char,vchar,date,datetime,boolean,blob,clob,null,isnull,isnotnull,true,false,=,!=,>,>=,<,<=

buildindex,rebuildindex,dropindex,primary,numbuckets,all,and,or,not,as

PaLanTir Names' Rule:
databaseName,tableName,attributeName,tableAlias : Name letter must in {A-Za-z}|{0-9}|"_",first letter must in {A-Za-z}, no longer than 50 characters.

PaLanTir Int Number Rule:[+|-]{0-9}*

PaLanTir Real Number Rule:[+|-]{0-9}*.{0-9}* ,[+|-]{0-9}*.{0-9}*{Ee}[+|-]{0-9}*

PaLanTir String Rule:Could be any letter,no longer than 255 characters.

PaLanTir Boolean Rule: true, false.

select sentence:
select *|tableName.*|attributeName|tableName.attributeName|tableAlias.*|tableAlias.attributeName[,...]
[into tableName] from tableName[ tableAlias][,...] 
[where attributeName|tableName.attributeName|tableAlias.attributeName 
{=|!=|<|<=|>=|> Int Number|Real Number|String|Boolean} 
| {isnull|isnotnull} 
| {=|!=|<|<=|>=|> tableName.attributeName|tableAlias.attributeName}]
