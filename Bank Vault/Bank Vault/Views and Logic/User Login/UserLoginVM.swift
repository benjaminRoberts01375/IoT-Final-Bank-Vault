// Ben Roberts

import AuthenticationServices
import CryptoKit
import SwiftUI

final class UserLoginVM {
    public func login(result: Result<ASAuthorization, Error>, dismiss: DismissAction) {
        switch result {
        case .success(let auth):
            print("Successful login")
            guard let credential = auth.credential as? ASAuthorizationAppleIDCredential else { return }
            UserM.shared.userID = String(SHA512.hash(data: Data(credential.user.utf8)).hashValue)   // For demo
            dismiss()
            
        case .failure(let error):
            print("Login failed: \(error.localizedDescription)")
        }
    }
}
