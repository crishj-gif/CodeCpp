class Solution {
public:
    int longestpalindromic(string s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            left--;
            right++;
        }
    
        return right - left - 1; 
    }

    string longestPalindrome(string s) {
        if (s.empty()) return "";
        int start = 0, end = 0;
        
        for (int i = 0; i < s.length(); i++) {
            // chuoi le
            int len1 = longestpalindromic(s, i, i);
            // chuoi chan
            int len2 = longestpalindromic(s, i, i + 1);
            
            int maxlen = max(len1, len2);
            if (maxlen > end - start) {
                start = i - (maxlen - 1) / 2;
                end = i + maxlen / 2;
            }
        }
        return s.substr(start, end - start + 1);
    }
};
