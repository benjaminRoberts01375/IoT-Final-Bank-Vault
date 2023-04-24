// Ben Roberts

import AuthenticationServices
import SwiftUI

final class UserLoginVM {
    public func login(result: Result<ASAuthorization, Error>, dismiss: DismissAction) {
        switch result {
        case .success(let auth):
            print("Successful login")
            guard let credential = auth.credential as? ASAuthorizationAppleIDCredential else { return }
            UserM.shared.userID = credential.user
            dismiss()
            
        case .failure(let error):
            print("Login failed: \(error.localizedDescription)")
        }
    }
}
