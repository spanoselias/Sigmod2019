import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.util.*;
import java.util.stream.Collectors;

public class Sort {

    private static int memcmp(byte[] a, byte[] b, int sz) {
        for (int i = 0; i < sz; i++) {
            if (a[i] != b[i]) {
                return Byte.toUnsignedInt(a[i]) - Byte.toUnsignedInt(b[i]);
            }
        }
        return 0;
    }

    public final static class Row implements Comparable<Row> {

        public static Row create(final byte[] key, final byte[] value) {
            return new Row(key, value);
        }

        public byte[] getKey() {
            return mKey;
        }

        public byte[] getValue() {
            return mValue;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Row key = (Row) o;
            return Arrays.equals(mKey, key.mKey);
        }

        @Override
        public int hashCode() {
            return Arrays.hashCode(mKey);
        }

        @Override
        public int compareTo(Row o) {

            return memcmp(mKey, o.getKey(), 10);
        }

        private Row(final byte[] key, final byte[] value) {
            mKey = key;
            mValue = value;
        }

        private final byte[] mKey;
        private final byte[] mValue;

    }

    public static void main(String[] args) throws IOException {

        long start = System.currentTimeMillis();
        sortingData();
        long finish = System.currentTimeMillis();
        long timeElapsed = finish - start;
        System.out.println("TimeElapse: " + timeElapsed);
    }

    private static void sortingData() throws IOException {
        final String fileName = "C:\\Users\\Elias\\Desktop\\Sigmod\\Sigmod2019\\src\\main\\resources\\input1";
        final File file = new File(fileName);

        final List<Row> list = new ArrayList<>();
        byte[] fileBytes = Files.readAllBytes(file.toPath());

        for (int i = 0; i < fileBytes.length; i += 100) {
            final byte[] key = Arrays.copyOfRange(fileBytes, i, i + 10);
            final byte[] value = Arrays.copyOfRange(fileBytes, i + 11, i + 101);
            list.add(Row.create(key, value));
        }

        final List<Row> res =
                list.parallelStream().sorted(sCmp).collect(Collectors.toList());

        try (FileOutputStream fos = new FileOutputStream("C:\\Users\\Elias\\Desktop\\Sigmod\\Sigmod2019\\src\\main\\resources\\output\\output")) {
            res.forEach(x -> {
                try {
                    fos.write(x.getKey());
                    fos.write(x.getValue());
                } catch (Throwable ex) {
                    System.out.println(ex.getMessage());
                }
            });
        } catch (Throwable ex) {
            System.out.println(ex.getMessage());
        }
    }

    private final static Comparator<Row> sCmp = (o1, o2) -> memcmp(o1.getKey(), o2.getKey(), 10);


}
