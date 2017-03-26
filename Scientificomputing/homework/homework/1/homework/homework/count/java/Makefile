
all:
	javac *.java
	jar cvf timing.jar *.class
	rm *.class

go:
	java -classpath timing.jar Counting

clean:
	rm *.class *.jar
