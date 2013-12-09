create table Artists (
	ArtistID INTEGER PRIMARY KEY,
	ArtistName TEXT);
	
create table CDs (
	CDID INTEGER PRIMARY KEY,
	ArtistID INTEGER NOT NULL,
	Title TEXT NOT NULL,
	Date TEXT);
	
insert into Artists (ArtistID,ArtistName) values (NULL,'Peter Gabriel');
insert into Artists (ArtistID,ArtistName) values (NULL,'Bruce Hornsby');
insert into Artists (ArtistID,ArtistName) values (NULL,'Lyle Lovett');
insert into Artists (ArtistID,ArtistName) values (NULL,'Beach Boys');

insert into CDs (CDID,ArtistID,Title,Date) values (NULL,1,'So','1984');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,1,'Us','1992');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,2,'The Way It Is','1986');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,2,'Scenes from the Southside','1990');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,1,'Security','1990');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,3,'Joshua Judges Ruth','1992');
insert into CDs (CDID,ArtistID,Title,Date) values (NULL,4,'Pet Sounds','1966');