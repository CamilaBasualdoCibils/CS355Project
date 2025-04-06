//void Merge(int *origin, unsigned halfSize, unsigned backSize);
//
//void mergesort(int *origin, unsigned startIndex, unsigned size)
//{
//    if(size < 2)
//        return;
//
//    unsigned halfSize = size / 2;
//    mergesort(origin, startIndex, halfSize);
//    mergesort(origin + halfSize, halfSize + 1, size - halfSize);
//    
//    Merge(origin, halfSize, size - halfSize);
//    
//}
//
//void Merge(int *origin, unsigned halfSize, unsigned backSize)
//{
//    int* merge = new int[halfSize + backSize];
//    unsigned i = 0, j = 0;
//    while(i < halfSize && j < backSize)
//    {
//        if(origin[i] < origin[j + halfSize])
//        {
//            merge[i + j] = origin[i];
//            ++i;
//        }
//        else
//        {
//            merge[i + j] = origin[j + halfSize];
//            ++j;
//        }
//    }
//
//    while ( i < halfSize )
//    {
//       merge[i + j] = origin[i];
//       ++i;
//    }
//
//    while ( j < backSize )
//    {
//       merge[i + j] = origin[j + halfSize];
//       ++j;
//    }
//
//    std::memcpy(origin, merge, (halfSize + backSize) * sizeof(int));
//    delete merge;
//}
