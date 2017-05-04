#!/usr/bin/python
# -*- coding: utf-8 -*-

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import sessionmaker

eng = create_engine('sqlite:///sqlite.db')

Base = declarative_base()


class Car(Base):
    __tablename__ = "Cars"

    Id = Column(Integer, primary_key=True)
    Name = Column(String)
    Price = Column(Integer)


def orm_entry():

    Base.metadata.bind = eng
    Base.metadata.create_all()

    Session = sessionmaker(bind=eng)
    ses = Session()

    ses.add_all(
        [Car(Id=1, Name='Audi', Price=52642),
         Car(Id=2, Name='Mercedes', Price=57127),
         Car(Id=3, Name='Skoda', Price=9000),
         Car(Id=4, Name='Volvo', Price=29000),
         Car(Id=5, Name='Bentley', Price=350000),
         Car(Id=6, Name='Citroen', Price=21000),
         Car(Id=7, Name='Hummer', Price=41400),
         Car(Id=8, Name='Volkswagen', Price=21600)])
    ses.commit()

    rs = ses.query(Car).all()

    for car in rs:
        print (car.Name, car.Price)