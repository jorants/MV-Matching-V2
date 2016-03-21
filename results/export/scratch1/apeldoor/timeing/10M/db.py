import peewee
from peewee import IntegerField,FloatField,BooleanField,ForeignKeyField

database = peewee.SqliteDatabase("data.db")




class Point(peewee.Model):
   V = IntegerField()
   E = IntegerField()
   done = BooleanField(default = False)
   class Meta:
      database = database

class Vline(peewee.Model):
   vval = IntegerField()
   class Meta:
      database = database


class Result(peewee.Model):
   point = ForeignKeyField(Point)
   time = FloatField()
   runtime = FloatField()   
   testType = IntegerField()

   class Meta:
      database = database

def create_yvals(max_val,steps):
    res = [0,max_val]
    for i in range(steps):
        res = do_ystep(res)
    return res

def do_ystep(p):
    res = []
    for i,x in enumerate(p[:-1]):
        res.append(x)
        new = int(x*0.75+p[i+1]*0.25)
        if new!=x:
           res.append(new)

    return res+[p[-1]]


 
      
def add_vline(V,step):
   maxe = V*(V-1)/2
   evals = create_yvals(maxe,step)
   for E in evals:
      P = Point(V = V,E = E)
      P.save()
   line = Vline(vval = V)
   line.save()

      
def get_undone_points():
   return Point.select().where(Point.done == False)



def to_test_type(name):
   return {
      "lemon":1,
      "libmv" :3,
      }[name]

def from_test_type(num):
   return [
      "lemon",
      "libmv",
   ][num]

def avr_time(p,test_type):
   results = Result.select().where((Result.point == p) & (Result.testType == to_test_type(test_type)))
   if results.count() == 0:
      return None
   
   return sum(map(lambda x: x.time,results))/results.count(),sum(map(lambda x: x.runtime,results))/results.count()


def main():
   try:
      database.drop_table(Result)
   except peewee.OperationalError:
      pass
   try:
      database.drop_table(Point)
   except peewee.OperationalError:
      pass
   try:
      database.drop_table(Vline)
   except peewee.OperationalError:
      pass


   database.create_tables([Result,Point,Vline])
   add_vline(2,0)
   add_vline(4,0)
   
   
if __name__ == "__main__":
  main()
